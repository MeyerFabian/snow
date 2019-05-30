#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../../../../src/core/buffer/buffer.hpp"
#include "../../../../src/test/test_util.hpp"
int main() {
  GLFWWindow();

  std::vector<GLuint> bufferValue = {1};
  Buffer<GLuint> uniform_buffer(BufferType::UNIFORM, BufferUsage::DYNAMIC_DRAW,
                                BufferLayout::AOS);

  uniform_buffer.transfer_to_gpu(bufferValue);
  uniform_buffer.gl_bind_base(0);
  executeTest(1, []() {
    return BenchmarkerGPU::getInstance().time("uniform_buffer", []() {});
  });
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  return 0;
}

