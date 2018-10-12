#include "scanPipeline.hpp"

void ScanPipeline::init(ScanTechnique::ScanData&& scan_data,
                        IO2BufferData&& io2) {
  ScanTechnique::ScanData scan_block_data = scan_data;
  scan_block_data.local_size = {16, 1, 1};
  IOBufferData io{io2.out2_buffer, io2.out2_buffer};
  localScan.init(std::move(scan_data), std::move(io2));
  blockScan.init(std::move(scan_block_data), std::move(io));
}

// 2 levels is enough for tiles(block) -> voxel(local)
void ScanPipeline::run(GLuint numValues) {
  buffer_size_local = numValues;
  BenchmarkerGPU::getInstance().time("Scan", [this]() {
    localScan.dispatch_with_barrier(buffer_size_local / 8);
  });
  buffer_size_block = blockScan.local_size.x;
  // std::cout << "block size: " << buffer_size_block << std::endl;
  BenchmarkerGPU::getInstance().time("ScanBlock", [this]() {
    blockScan.dispatch_with_barrier(buffer_size_block);
  });
}

