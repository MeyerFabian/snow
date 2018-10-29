#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../../../../snow/buffer/buffer.hpp"
#include "../../../../test/test_util.hpp"
int main() {
  GLFWWindow();

  Buffer<GLuint> buffer(BufferType::SSBO, BufferUsage::DYNAMIC_DRAW,
                        BufferLayout::AOS);

  buffer.resize_buffer(1024);
  buffer.gl_bind_base(0);
  executeTest(1, []() {
    return BenchmarkerGPU::getInstance().time("resize_buffer", []() {});
  });
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  return 0;
}

