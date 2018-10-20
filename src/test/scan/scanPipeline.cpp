#include "ScanPipeline.hpp"

void ScanPipeline::init(ScanTechnique::ScanData&& scan_data,
                        IOBufferData&& io2) {
  raking = scan_data.raking;

  buffer_size_local = scan_data.numVectors;

  block_size = scan_data.local_size.x * raking * 2;
  buffer_size_block = buffer_size_local / block_size;

  ScanTechnique::ScanData scan_block_data = scan_data;
  std::vector<BufferData> block_buffer = {io2.out_buffer[1]};

  IOBufferData io{{block_buffer}, {block_buffer}};
  scan_block_data.local_size = {1024, 1, 1};
  localScan.init(std::move(scan_data), std::move(io2));
  blockScan.init(std::move(scan_block_data), std::move(io));
}
GLuint ScanPipeline::get_scan_block_size() { return block_size; }

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

