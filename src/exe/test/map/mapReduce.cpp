#ifndef SHADER_FILENAME
#define SHADER_FILENAME "shader/compute/mapreduce/mapReduce.glsl"
#endif

#ifndef GLM_INC_GUARD
#define GLM_INC_GUARD
#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include <string>
#ifdef MAPREDUCE_SINGLE
#include "../../../test/soa_aos/gpu_in_out_structs_scalar.hpp"
#else
#include "../../../test/soa_aos/gpu_in_out_structs.hpp"
#endif
#endif

#ifndef NUMVECTORS
#define NUMVECTORS 1024 * 1024
#endif

#ifndef INIT_DATA
#define INIT_DATA PREC_VEC_TYPE(glm::ballRand(1.0), 0.0f)
#endif /* ifndef INIT_DATA */

#include <execution>
#include <numeric>
#include "../../../test/map/mapReduceTechnique.hpp"
//#undef MARKERS  // NVIDIA NSIGHT
#include "../../../snow/shader/shader.hpp"
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/map/mapReducePipeline.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/test_util.hpp"

#include "../../../test/BufferData.hpp"
int main() {
  GLFWWindow();
  GLuint numVectors = NUMVECTORS;
  LocalSize local_size = {1024, 1, 1};
  std::vector<Input> input_data;
  std::vector<Output> output_data_init;
  for (size_t i = 0; i < numVectors; i++) {
    input_data.push_back({INIT_DATA, INIT_DATA});
    // val here (50.0f) is actually overwritten, so this is another test
    output_data_init.push_back({0.0f, 0.0f});
  }
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif
  Buffer<Input> input(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
#ifdef MAPREDUCE_SINGLE
                      "shader/test/soa_aos/buffer_in_scalar.include.glsl"
#else
                      "shader/test/soa_aos/buffer_in.include.glsl"
#endif
  );

  input.transfer_to_gpu(input_data);
  input.gl_bind_base(1);

  auto output = std::make_shared<Buffer<Output>>(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,

#ifdef MAPREDUCE_SINGLE
      "shader/test/soa_aos/buffer_out_scalar.include.glsl"
#else
      "shader/test/soa_aos/buffer_out.include.glsl"
#endif

  );
  output->transfer_to_gpu(output_data_init);
  output->gl_bind_base(2);
  std::string filename = SHADER_FILENAME;
  auto in_v = BufferData("g_in", "in_v", input.get_buffer_info(), numVectors);
  auto out_g =
      BufferData("g_out", "out_g", output->get_buffer_info(), numVectors);

  IOBufferData io;
  // INPUT
  io.in_buffer.push_back(std::make_unique<BufferData>(in_v));

  // OUTPUT
  io.out_buffer.push_back(std::make_unique<BufferData>(out_g));

  MapReduceTechnique::MapReduceData reduce_data({
      filename,
      local_size,
      "PREC_SCAL_TYPE",
#ifdef MAPREDUCE_SINGLE
      "value",
#else
      "length(value)",
#endif
      "0",
      "left+right",
  });

  MapReducePipeline test;
  test.init(std::move(reduce_data), std::move(io));
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&numVectors, &test]() {
    executeTest(1, [&test, numVectors]() {
#ifndef NO_SEQUENTIAL_ADDS
      return test.run(numVectors);
#else
		    return test.runNoSeqAdd(numVectors);
#endif /* ifndef  */
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  auto sum_gpu = test.fetch_gpu_result(
      output, [](const auto& elem) { return elem.out_g; }, std::plus<>());
  BenchmarkerGPU::write_to_file("MapReduce");
  BenchmarkerGPU::write_to_file("Map");
  bench.write_to_file("MapReduceCPU");
#ifdef MAPREDUCE_SINGLE

  auto sum = std::transform_reduce(std::begin(input_data), std::end(input_data),
                                   0.0, std::plus<>(),
                                   [](const auto& elem) { return elem.in_v; });
#else
  auto sum = std::transform_reduce(
      std::begin(input_data), std::end(input_data), 0.0, std::plus<>(),
      [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.in_v)); });
#endif
  std::cout << "CPU map, CPU sum: " << sum << std::endl;
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(sum - sum_gpu) << std::endl;
  std::cout << "Relative Error: " << std::abs(sum - sum_gpu) / sum << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  bench.write_to_file("CPU time");
  // test.print();

  GLFWWindow::stop();
  return 0;
}

