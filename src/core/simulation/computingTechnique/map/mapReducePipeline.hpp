#ifndef MAPREDUCE_PIPELINE_H
#define MAPREDUCE_PIPELINE_H
#include <execution>
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../simulation/computingTechnique/map/mapReduceTechnique.hpp"
#include "../../../utils/math.hpp"
/*
 * Automates map reduce techniques s.t. they can reduce bigger sizes in a
 * pyramid scheme.
 */

class MapReducePipeline {
 public:
  void init(MapReduceTechnique::MapReduceData&& pipeline_data,
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
  MapReduceTechnique firstStep;
  MapReduceTechnique intermediateStep;
  MapReduceTechnique altStep;
  GLuint buffer_size_after;
};
#endif

