#ifndef GLM_INCLUDE_GUARD
#define GLM_INCLUDE_GUARD
#include "../../../snow/utils/defines.hpp"

#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "../../../snow/shader/shader.hpp"
#endif

#ifndef UNARY_OPERATION_GL
#include "../../src/snow/buffer/c_scan.hpp"

#define BUFFER_IN_NAME "shader/buffers/atomic_counter.include.glsl"
#define BUFFER_OUT_NAME "shader/buffers/scan_buffer.include.glsl"
#endif

#include <execution>
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/scan/scanTechnique.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  GLuint numValues;
  std::vector<GLuint> counter;
  std::vector<Scan> scan;
};

void test(testData& data) {
  GLFWWindow();
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif
  Buffer<GLuint> counter_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                                layout, BUFFER_IN_NAME);

  counter_buffer.transfer_to_gpu(data.counter);
  counter_buffer.gl_bind_base(ATOMIC_COUNTER_BINDING);

  Buffer<Scan> scan_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                           BUFFER_OUT_NAME);
  scan_buffer.transfer_to_gpu(data.scan);
  scan_buffer.gl_bind_base(SCAN_BUFFER_BINDING);

  ScanTechnique::ScanData scan_data({
      // alg-data
      "uint",
      "value",
      "0",
      "left+right",
      // IO2BufferData
      // NumValues
      data.numValues,
  });
  IO2BufferData io_data(
      {
          // in
          "counters",
          "Counter_i",
          counter_buffer.get_buffer_info(),
      },
      {
          // out
          "scans",
          "Scan_local_i",
          scan_buffer.get_buffer_info(),
      },
      {
          // out2
          "scans",
          "Scan_block_i",
          scan_buffer.get_buffer_info(),
      });

  IOBufferData io_block_data(
      {
          // in
          "scans",
          "Scan_block_i",
          scan_buffer.get_buffer_info(),
      },
      {
          // in
          "scans",
          "Scan_block_i",
          scan_buffer.get_buffer_info(),
      });
  auto scan_data_block = scan_data;
  auto scanShader = ScanTechnique();
  scanShader.init(std::move(scan_data), std::move(io_data));
  auto scan_block_shader = ScanTechnique();
  scan_block_shader.init(std::move(scan_data_block), std::move(io_block_data));

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&scanShader, &scan_block_shader,
                                      numValues = data.numValues]() {
    executeTest(1, [&scanShader, &scan_block_shader, numValues]() {
      BenchmarkerGPU::getInstance().time("Scan", [&scanShader, numValues]() {
        scanShader.dispatch_with_barrier(numValues);
      });
      BenchmarkerGPU::getInstance().time(
          "ScanBlock", [&scan_block_shader, numValues]() {
            scan_block_shader.dispatch_with_barrier(numValues / 32 / 2);
          });
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("Scan");
  bench.write_to_file("ScanCPU");
  auto scan = scan_buffer.transfer_to_cpu(std::size(data.scan));

  GLFWWindow::swapBuffers();
  data.scan = scan;
}

