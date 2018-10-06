#ifndef GLM_INCLUDE_GUARD
#define GLM_INCLUDE_GUARD
#include "../../../snow/utils/defines.hpp"

#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#endif

#ifndef UNARY_OPERATION_GL
#include "../../src/snow/particle/particle_exp.hpp"

#define BUFFER_IN_NAME "shader/buffers/atomic_counter.include.glsl"
#define BUFFER_OUT_NAME "shader/buffers/scan_buffer.include.glsl"
#endif

#include <execution>
#include "../../../snow/shader/shader.hpp"
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/scan/scanTechnique.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  GLuint numValues;
  std::vector<GLuint> counter;
  std::vector<GLuint> scan;
};

void test(testData& data) {
  GLFWWindow();
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif
  Buffer<GLuint> input(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                       BUFFER_IN_NAME);

  input.transfer_to_gpu(data.counter);
  input.gl_bind_base(ATOMIC_COUNTER_BINDING);

  Buffer<GLuint> output(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                        BUFFER_OUT_NAME);
  output.transfer_to_gpu(data.scan);
  output.gl_bind_base(SCAN_BUFFER_BINDING);

  MapTechnique::MapData map_data({
      // unary_op
      "0",
      // IOBufferData
      //   In
      "scans",
      "Scan_i",
      output.get_buffer_info(),
      //   Out
      "scans",
      "Scan_i",
      output.get_buffer_info(),
      // numValues
      data.numValues,
  });
  auto resetCounter = MapTechnique();
  resetCounter.init(std::move(map_data));

  ScanTechnique::ScanData scan_data({
      // alg-data
      "uint",
      "value",
      "0",
      "left+right",
      // IOBufferData
      // in
      "counters",
      "Counter_i",
      input.get_buffer_info(),
      // out
      "scans",
      "Scan_i",
      output.get_buffer_info(),
      // NumValues
      data.numValues,

  });
  auto scanShader = ScanTechnique();
  scanShader.init(std::move(scan_data));

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&scanShader, numValues = data.numValues,
                                      &resetCounter]() {
    executeTest(1, [&scanShader, &resetCounter, numValues]() {
      BenchmarkerGPU::getInstance().time(
          "resetCounter", [&resetCounter, numValues]() {
            resetCounter.dispatch_with_barrier(numValues);
          });

      BenchmarkerGPU::getInstance().time("Scan", [&scanShader, numValues]() {
        scanShader.dispatch_with_barrier(numValues);
      });
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("Scan");
  bench.write_to_file("ScanCPU");
  auto scan = output.transfer_to_cpu(std::size(data.scan));

  GLFWWindow::swapBuffers();
  data.scan = scan;
}

