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
  struct MapReduceData {
    std::string shader_filename;
    LocalSize local_size;
    std::string gl_unary_op;
    std::string gl_binary_op;
    std::string input;
    std::string output;
    std::string buffer_filename;
  };
  LocalSize local_size;
  void init(MapReduceData&& data) {
    local_size = data.local_size;

    auto shader =
        std::make_shared<Shader>(ShaderType::COMPUTE, data.shader_filename);

    shader->set_local_size(data.local_size);

    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
        {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
        {PreprocessorCmd::DEFINE, "UNARY_OP_RETURN_TYPE float"},
        {PreprocessorCmd::DEFINE, "INPUT(value) " + data.input},
        {PreprocessorCmd::DEFINE, "OUTPUT(value) " + data.output},
        {PreprocessorCmd::INCLUDE, "\"" + data.buffer_filename + "\""}};
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
    Input(glm::vec4 n_v) : v(n_v) {}
    glm::vec4 v;
  };

  struct Output {
    Output(float n_f) : f(n_f) {}

    float f;
  };
  MapReduceBuffers(size_t numVectors, LocalSize local_size)
      : input(std::make_shared<Buffer<Input>>(BufferType::SSBO,
                                              BufferUsage::STATIC_DRAW)),
        output(std::make_shared<Buffer<Output>>(BufferType::SSBO,
                                                BufferUsage::STATIC_DRAW)) {
    for (size_t i = 0; i < numVectors; i++) {
      input_data.emplace_back(glm::vec4(glm::ballRand(1.0f), 0.0f));
    }
    for (size_t i = 0; i < numVectors / local_size.x; i++) {
      output_data_init.emplace_back(0.0f);
    }

    input->transfer_to_gpu(input_data);
    input->gl_bind_base(1);

    output->transfer_to_gpu(output_data_init);
    output->gl_bind_base(2);
  };

  std::vector<Input> input_data;
  std::vector<Output> output_data_init;
  std::shared_ptr<Buffer<Input>> input;
  std::shared_ptr<Buffer<Output>> output;
};

class MapReduceTest {
 public:
  MapReduceTest(size_t numVectors, std::string name, std::string filename,
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
  void run(size_t index_size) const {
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

class MapReducePipeline {
 public:
  struct MapReducePipelineData {
    std::string filename;
    LocalSize local_size;
    std::string gl_unary_op;
    std::string gl_binary_op;
  };

  void init(MapReducePipelineData&& pipeline_data) {
    local_size = pipeline_data.local_size;
    MapReduce::MapReduceData reduction_data({
        pipeline_data.filename,
        pipeline_data.local_size,
        pipeline_data.gl_unary_op,
        pipeline_data.gl_binary_op,
        "g_in[value].v",
        "g_out[value].f",
        "shader/test/map/buffer.glsl",
    });
    from_immutable.init(std::move(reduction_data));
  }

  void run(size_t numVectors) {
    BenchmarkerGPU::getInstance().time(
        "MapReducePipeline 1st step", [this, &numVectors]() {
          from_immutable.dispatch_with_barrier(numVectors);
        });
  }
  template <typename Out, typename UnaryOp, typename BinaryOp>
  decltype(auto) fetch_gpu_result(size_t numVectors,
                                  std::shared_ptr<Buffer<Out>> buffer,
                                  UnaryOp u_op, BinaryOp b_op) {
    auto gpu_output = BenchmarkerGPU::getInstance().time(
        "MapBuffer", [this, &numVectors, &buffer]() {
          return buffer->transfer_to_cpu(numVectors / local_size.x);
        });

    return std::transform_reduce(std::execution::par, std::begin(gpu_output),
                                 std::end(gpu_output), 0.0f, b_op, u_op);
  }

 private:
  LocalSize local_size;
  MapReduce from_immutable;
  MapReduce to_mutable;
  MapReduce to_host;
};

#endif  // MAPREDUCE_H

