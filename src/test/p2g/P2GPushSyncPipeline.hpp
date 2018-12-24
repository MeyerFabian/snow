/*
#ifndef P2GPUSHSYNC_PIPELINE_H
#define P2GPUSHSYNC_PIPELINE_H
#include <execution>
#include "../../../snow/src/snow/utils/math.hpp"
#include "../IOBufferData.hpp"
#include "mapReduceTechnique.hpp"
class P2GPushSyncPipeline {
 public:
  void init(P2GPushSyncTechnique::MapReduceData&& pipeline_data,
            IOBufferData&& io);

  void run(GLuint numVectors);
  void runNoSeqAdd(GLuint numVectors);

  template <typename Out, typename UnaryOp, typename BinaryOp>
  decltype(auto) fetch_gpu_result(std::shared_ptr<Buffer<Out>> buffer,
                                  UnaryOp u_op, BinaryOp b_op) {
    auto gpu_output =
        BenchmarkerGPU::getInstance().time("MapBuffer", [this, &buffer]() {
          return buffer->transfer_to_cpu(buffer_size_after);
        });

    return std::transform_reduce(std::execution::par, std::begin(gpu_output),
                                 std::end(gpu_output), 0.0, b_op, u_op);
  }

 private:
  LocalSize local_size;
  P2GPushSyncTechnique firstStep;
  P2GPushSyncTechnique intermediateStep;
  P2GPushSyncTechnique altStep;
  GLuint buffer_size_after;
};
#endif
*/
