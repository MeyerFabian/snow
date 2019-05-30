#ifndef SCAN_PIPELINE_H
#define SCAN_PIPELINE_H
#include <execution>
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../utils/benchmarker.hpp"
#include "../../../utils/math.hpp"
#include "scanTechnique.hpp"
#include "scanWriteBackTechnique.hpp"

/*
 * Automates scan techniques to allow for higher data sizes.
 * Handles sizes up to (raking*block_size)^2.
 */
class ScanPipeline {
 public:
  void init(ScanTechnique::ScanData&&, IOBufferData&& io);

  void initDirectWriteBack(ScanTechnique::ScanData&&, IOBufferData&& io);
  void run(GLuint numVectors);

  GLuint get_scan_block_size();

 private:
  LocalSize local_size;
  ScanTechnique blockScan;
  ScanTechnique localScan;
  ScanWriteBackTechnique writeBack;
  GLuint buffer_size_local;
  GLuint buffer_size_block;
  GLuint block_size;
  GLuint raking;
  GLuint numValues;
  bool dw_back;
};
#endif

