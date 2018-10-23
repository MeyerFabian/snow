#ifndef SCAN_PIPELINE_H
#define SCAN_PIPELINE_H
#include <execution>
#include "../../../snow/src/snow/utils/math.hpp"
#include "../../snow/utils/benchmarker.hpp"
#include "../IOBufferData.hpp"
#include "scanTechnique.hpp"
#include "scanWriteBackTechnique.hpp"

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

