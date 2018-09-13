#include "../../../test/map/mapReduce_d.hpp"
int main() {
  GLFWWindow();

  size_t numVectors = 1'024 * 1'024;
  LocalSize local_size = {1024, 1, 1};

  MapReduceTest test(numVectors, "MapReduceSequential",
                     "shader/test/map/mapReduceSequential.glsl", local_size);

#ifdef MARKERS
  while (GLFWWindow::shouldClose()) {
#endif
    BenchmarkerGPU::getInstance().time(
        test.name, [&test, numVectors]() { test.run(numVectors); });

    GLFWWindow::clear();
    GLFWWindow::swapBuffers();
    BenchmarkerGPU::getInstance().collect_times_last_frame();
#ifdef MARKERS
  }
#endif
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("MapReduce");
  test.print();

  GLFWWindow::stop();
  return 0;
}

