#ifndef SCAN_PIPELINE_H
#define SCAN_PIPELINE_H
#include <execution>
#include "../../../snow/src/snow/utils/math.hpp"
#include "../../snow/utils/benchmarker.hpp"
#include "../IOBufferData.hpp"
#include "scanTechnique.hpp"
class ScanPipeline {
 public:
  void init(ScanTechnique::ScanData&&, IO2BufferData&& io);

  void run(GLuint numVectors);
  void runNoSeqAdd(GLuint numVectors);

 private:
  LocalSize local_size;
  ScanTechnique blockScan;
  ScanTechnique localScan;
  GLuint buffer_size_local;
  GLuint buffer_size_block;
  GLuint raking;
  GLuint numValues;
};
#endif

