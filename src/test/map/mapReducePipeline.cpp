#include "mapReducePipeline.hpp"

void MapReducePipeline::init(MapReducePipelineData&& pipeline_data) {
  local_size = pipeline_data.local_size;
  MapReduceTechnique::MapReduceData reduction_data_1st({
      pipeline_data.filename,
      pipeline_data.local_size,
      pipeline_data.gl_unary_op,
      pipeline_data.gl_binary_op,
      "g_in[value].v",
      "g_out[value].f",
      "shader/test/map/buffer.glsl",
  });

  MapReduceTechnique::MapReduceData reduction_data_2nd = reduction_data_1st;
  reduction_data_2nd.gl_unary_op = "value";
  reduction_data_2nd.input = "g_out[value].f";
  reduction_data_2nd.local_size = {1, 1, 1};
  firstStep.init(std::move(reduction_data_1st));
  intermediateStep.init(std::move(reduction_data_2nd));
}

void MapReducePipeline::run(GLuint numVectors) {
  BenchmarkerGPU::getInstance().time(
      "MapReducePipeline 1st step", [this, numVectors]() {
        GLuint buffer_size_before = numVectors;
        buffer_size_after = buffer_size_before / local_size.x / 128;
        firstStep.dispatch_with_barrier(
            {buffer_size_before, buffer_size_after});
      });
  /*
  BenchmarkerGPU::getInstance().time("MapReducePipeline 2nd step", [this]() {
    GLuint buffer_size_before = buffer_size_after;
    buffer_size_after /= 8;
    intermediateStep.dispatch_with_barrier(
        {buffer_size_before, buffer_size_after});
  });
  */
}

