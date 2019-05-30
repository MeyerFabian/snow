#include "ScanPipeline.hpp"
/*
 * 1.) Execute normal scan technique local_scan with input scan_data.
 * This also allows for a unary operation.
 * 2.) Executes block scan without applying the unary_op again.
 *
 * @scan_data
 *   .raking       : number of sequential elements per thread
 *   .gl_unary_op  : expects "value" in map-op.
 * @io2
 *   .in_buffer [0]: counter e.g. something to perform a scan on
 *   .out_buffer[0]: local scan, first level pyramid
 *   .out_buffer[1]: block scan, 2nd level pyramid
 */
void ScanPipeline::init(ScanTechnique::ScanData&& scan_data,
                        IOBufferData&& io2) {
  raking = scan_data.raking;

  buffer_size_local = scan_data.numVectors;

  block_size = scan_data.local_size.x * raking * 2;
  buffer_size_block = buffer_size_local / block_size;

  ScanTechnique::ScanData scan_block_data = scan_data;
  scan_block_data.gl_unary_op = "value";

  // std::vector<BufferData> block_buffer = {io2.out_buffer[1]};

  IOBufferData io;
  // in
  io.in_buffer.push_back(io2.out_buffer[1]->cloneBufferDataInterface());
  // out
  io.out_buffer.push_back(io2.out_buffer[1]->cloneBufferDataInterface());

  scan_block_data.local_size = {1024, 1, 1};
  localScan.init(std::move(scan_data), std::move(io2));
  blockScan.init(std::move(scan_block_data), std::move(io));
}

/*
 * 1.) Execute normal scan technique local_scan with input scan_data.
 * This also allows for a unary operation.
 * 2.) Executes block scan without applying the unary_op again.
 * 3.) Writes back the block scan into respective local scan.
 *
 * @scan_data
 *   .raking       : number of sequential elements per thread
 *   .gl_unary_op  : expects "value" in map-op.
 * @io2
 *   .in_buffer [0]: counter e.g. something to perform a scan on
 *   .out_buffer[0]: local scan, first level pyramid
 *   .out_buffer[1]: block scan, 2nd level pyramid
 */
void ScanPipeline::initDirectWriteBack(ScanTechnique::ScanData&& scan_data,
                                       IOBufferData&& io2) {
  // set bool flag for dispatch run
  dw_back = true;
  // ScanWriteBack so block_sum gets written back to locals
  ScanWriteBackTechnique::ScanWriteBackData scan_write_back{
      scan_data.gl_binary_op,
      scan_data.numVectors,
  };

  IOBufferData io;
  // in
  io.in_buffer.push_back(io2.out_buffer[1]->cloneBufferDataInterface());
  io.out_buffer.push_back(io2.out_buffer[0]->cloneBufferDataInterface());

  ScanPipeline::init(std::move(scan_data), std::move(io2));

  // get_scan_block_size() is ready only after pipeline init
  scan_write_back.scanBlockSize = get_scan_block_size();

  writeBack.init(std::move(scan_write_back), std::move(io));
}

GLuint ScanPipeline::get_scan_block_size() { return block_size; }

// Runs either with or without direct write back.
void ScanPipeline::run(GLuint numValues) {
  BenchmarkerGPU::getInstance().time("Scan", [this]() {
    localScan.dispatch_with_barrier({buffer_size_local, buffer_size_block});
  });
  BenchmarkerGPU::getInstance().time("ScanBlock", [this]() {
    blockScan.dispatch_with_barrier({buffer_size_block, 1});
  });

  if (dw_back) {
    BenchmarkerGPU::getInstance().time("WriteBack", [this, &numValues]() {
      writeBack.dispatch_with_barrier(numValues);
    });
  }
}

