#include "../../../../src/core/utils/defines.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <iterator>
#include <vector>
#include "../../../../src/core/buffer/buffer.hpp"
#include "../../../../src/core/utils/benchmarker.hpp"
#include "../../../../src/test/test_util.hpp"
struct Particle_AOS {
  glm::vec4 x;
  glm::vec4 v;
  float k;
  float l;
  float m;
  float n;
};

template <typename T, typename std::enable_if_t<
                          std::is_arithmetic_v<std::decay_t<T>>, int> = 0>
float norm1(T t) {
  return t;
}
template <typename T, typename std::enable_if_t<
                          !std::is_arithmetic_v<std::decay_t<T>>, int> = 0>
float norm1(T t) {
  return glm::l2Norm(glm::vec3(t));
}

int main() {
  GLFWWindow();
  std::vector<Particle_AOS> input_data = {
      {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, 9.0, 10.0, 11.0, 12.0},
      {{13.0, 14.0, 15.0, 16.0},
       {17.0, 18.0, 19.0, 20.0},
       21.0,
       22.0,
       23.0,
       24.0},
  };

  Buffer<Particle_AOS> input_Buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW,
      BufferLayout::SOA);  // Convert to SOA for GPU

  input_Buffer.transfer_to_gpu(input_data);
  input_Buffer.gl_bind_base(1);

  executeTest(1'000, []() {
    return BenchmarkerGPU::getInstance().time("map", []() {});
  });
  std::vector<Particle_AOS> output_data(
      input_Buffer.transfer_to_cpu(std::size(input_data)));
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  float sum_in = 0.0f;
  float sum_out = 0.0f;
  for (size_t i = 0; i < std::size(input_data); i++) {
    boost::pfr::for_each_field(input_data[i], [&sum_in](const auto& field) {
      sum_in += norm1(field);
    });
    boost::pfr::for_each_field(output_data[i], [&sum_out](const auto& field) {
      sum_out += norm1(field);
    });
  }
  std::cerr << "AOS sum: " << sum_in << std::endl;
  std::cerr << "SOA sum: " << sum_out << std::endl;
  std::cerr << "Diff: " << sum_in - sum_out << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  return 0;
}

