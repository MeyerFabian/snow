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

  void init(MapReducePipelineData&& pipeline_data);

  void run(GLuint numVectors);

  template <typename Out, typename UnaryOp, typename BinaryOp>
  decltype(auto) fetch_gpu_result(GLuint numVectors,
                                  std::shared_ptr<Buffer<Out>> buffer,
                                  UnaryOp u_op, BinaryOp b_op) {
    auto gpu_output = BenchmarkerGPU::getInstance().time(
        "MapBuffer", [this, &numVectors, &buffer]() {
          return buffer->transfer_to_cpu(buffer_size_after);
        });

    return std::transform_reduce(std::execution::par, std::begin(gpu_output),
                                 std::end(gpu_output), 0.0f, b_op, u_op);
  }

 private:
  LocalSize local_size;
  MapReduceTechnique firstStep;
  MapReduceTechnique intermediateStep;
  MapReduceTechnique to_host;
  GLuint buffer_size_after;
};
#endif

