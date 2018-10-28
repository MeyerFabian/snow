#include "mapReducePipeline.hpp"

void MapReducePipeline::init(MapReduceTechnique::MapReduceData&& pipeline_data,
                             IOBufferData&& io) {
  local_size = pipeline_data.local_size;

  MapReduceTechnique::MapReduceData reduction_data_2nd = pipeline_data;
  reduction_data_2nd.gl_unary_op = "value";
  IOBufferData io2 = io.clone();
  io2.in_buffer[0]->setName(io.out_buffer[0]->getName());

  io2.in_buffer[0]->setVariable(io.out_buffer[0]->getVariable());

  reduction_data_2nd.local_size = {1, 1, 1};

  MapReduceTechnique::MapReduceData reduction_data_alt = pipeline_data;
  reduction_data_2nd.local_size = {1, 1, 1};
  IOBufferData io_alt = io.clone();

  firstStep.init(std::move(pipeline_data), std::move(io));
  intermediateStep.init(std::move(reduction_data_2nd), std::move(io2));
  altStep.init(std::move(reduction_data_alt), std::move(io_alt));
}

void MapReducePipeline::run(GLuint numVectors) {
  if (numVectors > 1024) {
    BenchmarkerGPU::getInstance().time(
        "MapReducePipeline Map+Reduce", [this, numVectors]() {
          // After tests we aim at a grid/dispatch size between 8-16
          GLuint buffer_size_before = numVectors;
          GLuint global_loads_per_thread =
              buffer_size_before / local_size.x / 8;
          if (!IsPowerOfTwo(buffer_size_before)) {
            global_loads_per_thread = next_pow_2(global_loads_per_thread);
          }
          buffer_size_after =
              buffer_size_before / local_size.x / global_loads_per_thread;
          firstStep.dispatch_with_barrier(
              {buffer_size_before, buffer_size_after});
        });

    std::string log_name = "MapReducePipeline Reduce: (" +
                           std::to_string(buffer_size_after) + " -> 1)";
    BenchmarkerGPU::getInstance().time(log_name, [this]() {
      GLuint buffer_size_before = buffer_size_after;
      buffer_size_after = 1;
      intermediateStep.dispatch_with_barrier(
          {buffer_size_before, buffer_size_after});
    });
  } else {
    std::string log_name =
        "MapReducePipeline altStep: (" + std::to_string(numVectors) + " -> 1)";
    BenchmarkerGPU::getInstance().time(log_name, [this, &numVectors]() {
      GLuint buffer_size_before = numVectors;
      buffer_size_after = 1;
      altStep.dispatch_with_barrier({buffer_size_before, buffer_size_after});
    });
  }
}
void MapReducePipeline::runNoSeqAdd(GLuint numVectors) {
  buffer_size_after = numVectors / local_size.x;
  BenchmarkerGPU::getInstance().time(
      "MapReducePipeline Map+Reduce",
      [this, numVectors]() { firstStep.dispatch_with_barrier(numVectors); });
}

