#include <execution>
#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/rendering/GLFWWindow.hpp"
#include "../../snow/shader/technique.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
class MapTest : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};
  void init() {
    auto shader = std::make_shared<Shader>(ShaderType::COMPUTE,
                                           "shader/compute/mapreduce/map.glsl");

    shader->set_local_size(local_size);

    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "UNARY_OP(value) length(value)"},
        {PreprocessorCmd::DEFINE, "INPUT(value) g_in[value].v"},
        {PreprocessorCmd::DEFINE, "OUTPUT(value) g_out[value].f"},
        {PreprocessorCmd::INCLUDE, "\"shader/test/map/buffer.glsl\""}};

    shader->add_cmds(vec.begin(), vec.end());

    Technique::add_shader(std::move(shader));
    Technique::upload();
    Technique::use();
  }
  void dispatch_with_barrier(size_t numVectors) {
    glDispatchCompute(numVectors / local_size.x, 1 / local_size.y,
                      1 / local_size.z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
};

int main() {
  size_t numVectors = 1'024 * 1'024;
  GLFWWindow();
  struct Input {
    Input(glm::vec4 n_v) : v(n_v) {}
    glm::vec4 v;
  };

  struct Output {
    Output(float n_f) : f(n_f) {}
    float f;
  };

  std::vector<Input> input_data;
  std::vector<Output> output_data_init;

  for (size_t i = 0; i < numVectors; i++) {
    input_data.emplace_back(glm::vec4(glm::ballRand(1.0f), 0.0f));
    output_data_init.emplace_back(0.0f);
  }

  Buffer<Input> input(BufferType::SSBO);
  input.transfer_to_gpu(input_data);
  input.gl_bind_base(1);

  Buffer<Output> output(BufferType::SSBO);
  output.transfer_to_gpu(output_data_init);
  output.gl_bind_base(2);

  auto test = MapTest();
  test.init();

  while (GLFWWindow::shouldClose()) {
    BenchmarkerGPU::getInstance().time("Map", [&test, numVectors]() {
      test.dispatch_with_barrier(numVectors);
    });

    GLFWWindow::clear();
    GLFWWindow::swapBuffers();
    BenchmarkerGPU::getInstance().collect_times_last_frame();
  }

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("Map");

  auto m(output.transfer_to_cpu());

  auto sum = std::transform_reduce(
      std::begin(input_data), std::end(input_data), 0.0f, std::plus<>(),
      [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.v)); });

  auto sum_gpu =
      std::transform_reduce(std::begin(m), std::end(m), 0.0f, std::plus<>(),
                            [](const auto& elem) { return elem.f; });
  std::cout << "CPU map, CPU sum: " << sum << std::endl;
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(sum - sum_gpu) << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  return 0;
}

