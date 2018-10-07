#ifndef SHADER_FILENAME
#define SHADER_FILENAME "shader/compute/mapreduce/mapReduce.glsl"
#endif

#ifndef GLM_INC_GUARD
#define GLM_INC_GUARD
#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include <string>
#include "../../../test/soa_aos/gpu_in_out_structs.hpp"
#endif

#ifndef NUMVECTORS
#define NUMVECTORS 1024 * 1024
#endif

#ifndef INIT_DATA
#define INIT_DATA PREC_VEC_TYPE(glm::ballRand(1.0f), 0.0f)
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
                      "shader/test/soa_aos/buffer_in.include.glsl");

  input.transfer_to_gpu(input_data);
  input.gl_bind_base(1);

  auto output = make_shared<Buffer<Output>>(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,

      "shader/test/soa_aos/buffer_out.include.glsl"

  );
  output->transfer_to_gpu(output_data_init);
  output->gl_bind_base(2);
  std::string filename = SHADER_FILENAME;
  MapReduceTechnique::MapReduceData reduce_data({
      filename,
      local_size,
      "PREC_SCAL_TYPE",
      "length(value)",
      "0",
      "left+right",
      // IOBufferData
      IOBufferData(
          {
              //   In
              "g_in",
              "in_v",
              input.get_buffer_info(),
          },
          {
              //   Out
              "g_out",
              "out_g",
              output->get_buffer_info(),
          }),
      // numVectors
      numVectors,
  });
  MapReducePipeline test;
  test.init(std::move(reduce_data));
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&numVectors, &test]() {
    executeTest(1'000, [&test, numVectors]() {
#ifndef NO_SEQUENTIAL_ADDS
      return test.run(numVectors);
#else
		    return test.runNoSeqAdd(numVectors);
#endif /* ifndef  */
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  float sum_gpu = test.fetch_gpu_result(
      output, [](const auto& elem) { return elem.out_g; }, std::plus<>());
  BenchmarkerGPU::write_to_file("MapReduce");
  BenchmarkerGPU::write_to_file("Map");
  bench.write_to_file("MapReduceCPU");
  auto sum = std::transform_reduce(
      std::begin(input_data), std::end(input_data), 0.0f, std::plus<>(),
      [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.in_v)); });

  std::cout << "CPU map, CPU sum: " << sum << std::endl;
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(sum - sum_gpu) << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  bench.write_to_file("CPU time");
  // test.print();

  GLFWWindow::stop();
  return 0;
}

