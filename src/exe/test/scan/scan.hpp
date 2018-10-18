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
#ifndef SHADER_NAME
#define SHADER_NAME "shader/compute/preprocess/scan.glsl"
#endif
#ifndef UNARY_OPERATION_GL
#include "../../src/snow/buffer/c_scan.hpp"

#define BUFFER_IN_NAME "shader/buffers/atomic_counter.include.glsl"
#define BUFFER_OUT_NAME "shader/buffers/scan_buffer.include.glsl"
#endif

#include <execution>
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/scan/ScanPipeline.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  LocalSize local_size;
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

  ScanTechnique::ScanData scan_data{
      // local_size
      data.local_size,
      SHADER_NAME,
      // alg-data
      "uint",
      "value",
      "0",
      "left+right",
      data.numValues,
#ifndef NO_SEQUENTIAL_ADDS
      MULTIPLE_ELEMENTS,
#endif
  };
  IO2BufferData io_data{{
                            // in
                            "counters",
                            "Counter_i",
                            counter_buffer.get_buffer_info(),
                            data.numValues,
                        },
                        {
                            // out
                            "scans",
                            "Scan_local_i",
                            scan_buffer.get_buffer_info(),
                            data.numValues,
                        },
                        {
                            // out2
                            "scans",
                            "Scan_block_i",
                            scan_buffer.get_buffer_info(),
                            data.numValues,
                        }};

  auto scanPipeline = ScanPipeline();
  scanPipeline.init(std::move(scan_data), std::move(io_data));

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent",
             [&scanPipeline, numValues = data.numValues]() {
               executeTest(1, [&scanPipeline, numValues]() {
#ifndef NO_SEQUENTIAL_ADDS
                 scanPipeline.run(numValues);
#else 
		 scanPipeline.runNoSeqAdd(numValues);
#endif
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

