#ifndef MAPREDUCE_SINGLE_H
#define MAPREDUCE_SINGLE_H
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/benchmarker.hpp"
#include "mapReduceBuffers.hpp"
#include "mapReduceTechnique.hpp"

class MapReduceTest {
 public:
  MapReduceTest(GLuint numVectors, std::string name, std::string filename,
                LocalSize local_size)
      : name(name), buffer(numVectors, local_size) {
    shaderprogram.init({
        filename,
        local_size,
        "length(value)",
        "left+right",
        "g_in[value].v",
        "g_out[value].f",
        "shader/test/map/buffer.glsl",
    });
  }
  void run(GLuint index_size) const {
    shaderprogram.dispatch_with_barrier(index_size);
  }

  void print() const {
    auto gpu_output = BenchmarkerGPU::getInstance().time("MapBuffer", [this]() {
      return buffer.output->transfer_to_cpu(std::size(buffer.output_data_init));
    });
    auto sum = std::transform_reduce(
        std::execution::par, std::begin(buffer.input_data),
        std::end(buffer.input_data), 0.0f, std::plus<>(),
        [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.v)); });

    auto sum_gpu = std::transform_reduce(
        std::execution::par, std::begin(gpu_output), std::end(gpu_output), 0.0f,
        std::plus<>(), [](const auto& elem) { return elem.f; });

    std::cout << name << std::endl;
    std::cout << "CPU map, CPU sum: " << sum << std::endl;
    std::cout << "GPU map, GPU sum: " << sum_gpu << std::endl;
    float abs_error = std::abs(sum - sum_gpu);
    float rel_error = abs_error / sum;
    std::cout << "Absolute error: " << abs_error << std::endl;
    std::cout << "Relative error: " << rel_error << std::endl;
  }
  std::string name;
  MapReduce shaderprogram;
  MapReduceBuffers buffer;
};
#endif

