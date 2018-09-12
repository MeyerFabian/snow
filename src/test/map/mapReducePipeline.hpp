#ifndef MAPREDUCE_PIPELINE_H
#define MAPREDUCE_PIPELINE_H
#include "mapReduceTechnique.hpp"
class MapReducePipeline {
 public:
  struct MapReducePipelineData {
    std::string filename;
    LocalSize local_size;
    std::string gl_unary_op;
    std::string gl_binary_op;
  };

  void init(MapReducePipelineData&& pipeline_data) {
    local_size = pipeline_data.local_size;
    MapReduce::MapReduceData reduction_data({
        pipeline_data.filename,
        pipeline_data.local_size,
        pipeline_data.gl_unary_op,
        pipeline_data.gl_binary_op,
        "g_in[value].v",
        "g_out[value].f",
        "shader/test/map/buffer.glsl",
    });
    from_immutable.init(std::move(reduction_data));
  }

  void run(GLuint numVectors) {
    BenchmarkerGPU::getInstance().time(
        "MapReducePipeline 1st step", [this, &numVectors]() {
          from_immutable.dispatch_with_barrier(numVectors / 2, {numVectors, 1});
        });
  }
  template <typename Out, typename UnaryOp, typename BinaryOp>
  decltype(auto) fetch_gpu_result(GLuint numVectors,
                                  std::shared_ptr<Buffer<Out>> buffer,
                                  UnaryOp u_op, BinaryOp b_op) {
    auto gpu_output = BenchmarkerGPU::getInstance().time(
        "MapBuffer", [this, &numVectors, &buffer]() {
          return buffer->transfer_to_cpu(numVectors / local_size.x);
        });

    return std::transform_reduce(std::execution::par, std::begin(gpu_output),
                                 std::end(gpu_output), 0.0f, b_op, u_op);
  }

 private:
  LocalSize local_size;
  MapReduce from_immutable;
  MapReduce to_mutable;
  MapReduce to_host;
};
#endif

