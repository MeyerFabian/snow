#include "ScanPipeline.hpp"

void ScanPipeline::init(ScanTechnique::ScanData&& scan_data,
                        IO2BufferData&& io2) {
  raking = scan_data.raking;

  buffer_size_local = scan_data.numVectors;
  buffer_size_block = buffer_size_local / scan_data.local_size.x / raking / 2;

  ScanTechnique::ScanData scan_block_data = scan_data;
  IOBufferData io{io2.out2_buffer, io2.out2_buffer};
  scan_block_data.local_size = {1024, 1, 1};
  localScan.init(std::move(scan_data), std::move(io2));
  blockScan.init(std::move(scan_block_data), std::move(io));
}

// 2 levels is enough for tiles(block) -> voxel(local)
void ScanPipeline::run(GLuint numValues) {
  BenchmarkerGPU::getInstance().time("Scan", [this]() {
    localScan.dispatch_with_barrier({buffer_size_local, buffer_size_block});
  });
  BenchmarkerGPU::getInstance().time("ScanBlock", [this]() {
    blockScan.dispatch_with_barrier({buffer_size_block, 1});
  });
}

// 2 levels is enough for tiles(block) -> voxel(local)
void ScanPipeline::runNoSeqAdd(GLuint numValues) {
  BenchmarkerGPU::getInstance().time("Scan", [this]() {
    localScan.dispatch_with_barrier({buffer_size_local, buffer_size_block});
  });

  // std::cout << "block size: " << buffer_size_block << std::endl;
  BenchmarkerGPU::getInstance().time("ScanBlock", [this]() {
    blockScan.dispatch_with_barrier({buffer_size_block, 1});
  });
}

