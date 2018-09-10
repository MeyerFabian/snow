#include "mapReduce_d.hpp"
int main() {
  GLFWWindow();

  size_t numVectors = 1'024 * 1'024;
  LocalSize local_size = {1024, 1, 1};

  MapReduceTest test(numVectors, "MapReduceSequential",
                             "shader/compute/mapreduce/mapReduce.glsl",
                             local_size);

  while (GLFWWindow::shouldClose()) {
    BenchmarkerGPU::getInstance().time(
        test.name, [&test, numVectors]() { test.run(numVectors); });

    GLFWWindow::clear();
    GLFWWindow::swapBuffers();
    BenchmarkerGPU::getInstance().collect_times_last_frame();
  }

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("MapReduce");
  test.print();

  GLFWWindow::stop();
  return 0;
}

