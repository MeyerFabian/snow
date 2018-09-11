#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include <execution>
#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/rendering/GLFWWindow.hpp"
#include "../../snow/shader/technique.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <chrono>
#include <glm/gtx/norm.hpp>
#include <thread>
#include "../../snow/utils/benchmarker.hpp"
class MapReduce : public Technique {
 public:
  LocalSize local_size;
  void init(std::string filename, LocalSize ls) {
    local_size = ls;

    auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, filename);

    shader->set_local_size(local_size);

    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "UNARY_OP(value) length(value)"},
        {PreprocessorCmd::DEFINE, "UNARY_OP_RETURN_TYPE double"},
        {PreprocessorCmd::DEFINE, "INPUT(value) g_in[value].v"},
        {PreprocessorCmd::DEFINE, "OUTPUT(value) g_out[value].f"},
        {PreprocessorCmd::INCLUDE, "\"shader/test/map/buffer_d.glsl\""}};
    shader->add_cmds(vec.begin(), vec.end());

    Technique::add_shader(std::move(shader));
    Technique::upload();
    Technique::use();
  }
  void dispatch_with_barrier(size_t numVectors) const {
    glDispatchCompute(numVectors / local_size.x, 1 / local_size.y,
                      1 / local_size.z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
};
class MapReduceBuffers {
 public:
  struct Input {
    Input(glm::dvec4 n_v) : v(n_v) {}
    glm::dvec4 v;
  };

  struct Output {
    Output(double n_f) : f(n_f) {}
    double f;
  };

  MapReduceBuffers(size_t numVectors, LocalSize local_size)
      : input(Buffer<Input>(BufferType::SSBO, BufferUsage::STATIC_DRAW)),
        output(Buffer<Output>(BufferType::SSBO, BufferUsage::DYNAMIC_READ)) {
    for (size_t i = 0; i < numVectors; i++) {
      input_data.emplace_back(glm::dvec4(glm::ballRand(1.0f), 0.0f));
    }
    for (size_t i = 0; i < numVectors / local_size.x; i++) {
      output_data_init.emplace_back(0.0);
    }

    input.transfer_to_gpu(input_data);
    input.gl_bind_base(1);

    output.transfer_to_gpu(output_data_init);
    output.gl_bind_base(2);
  };

  std::vector<Input> input_data;
  std::vector<Output> output_data_init;
  Buffer<Input> input;
  Buffer<Output> output;
};

class MapReduceTest {
 public:
  MapReduceTest(size_t numVectors, std::string name, std::string filename,
                LocalSize local_size)
      : name(name), buffer(numVectors, local_size) {
    shaderprogram.init(filename, local_size);
  }
  void run(size_t index_size) const {
    shaderprogram.dispatch_with_barrier(index_size);
  }

  void print() const {
    auto gpu_output =
        buffer.output.transfer_to_cpu(std::size(buffer.output_data_init));
    auto sum = std::transform_reduce(
        std::execution::par, std::begin(buffer.input_data),
        std::end(buffer.input_data), 0.0, std::plus<>(),
        [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.v)); });

    auto sum_gpu = std::transform_reduce(
        std::execution::par, std::begin(gpu_output), std::end(gpu_output), 0.0,
        std::plus<>(), [](const auto& elem) { return elem.f; });

    std::cout << name << std::endl;
    std::cout << "CPU map, CPU sum: " << sum << std::endl;
    std::cout << "GPU map, GPU sum: " << sum_gpu << std::endl;
    double abs_error = std::abs(sum - sum_gpu);
    double rel_error = abs_error / sum;
    std::cout << "Absolute error: " << abs_error << std::endl;
    std::cout << "Relative error: " << rel_error << std::endl;
  }
  std::string name;
  MapReduce shaderprogram;
  MapReduceBuffers buffer;
};

#endif  // MAPREDUCE_H

