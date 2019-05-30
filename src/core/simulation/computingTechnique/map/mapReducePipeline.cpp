#include "mapReducePipeline.hpp"

/*
 *@io.
 *   in_buffer [0]: buffer to execute reduction on
 *   out_buffer[0]: buffer to store reduced values in after first reduction
 */
void MapReducePipeline::init(MapReduceTechnique::MapReduceData&& pipeline_data,
                             IOBufferData&& io) {
  local_size = pipeline_data.local_size;

  MapReduceTechnique::MapReduceData reduction_data_2nd = pipeline_data;
  // The reductions up in the pyramid don't apply the unary operation again.
  reduction_data_2nd.gl_unary_op = "value";
  reduction_data_2nd.local_size = {1, 1, 1};

  // And after the first reduction we only do subsequent reductions in the
  // "reduction" buffer i.e. output buffer+variable of the first pass
  IOBufferData io2 = io.clone();
  io2.in_buffer[0]->setName(io.out_buffer[0]->getName());
  io2.in_buffer[0]->setVariable(io.out_buffer[0]->getVariable());

  // the altered one comes into effect if buffer size is below 1024
  MapReduceTechnique::MapReduceData reduction_data_alt = pipeline_data;

  IOBufferData io_alt = io.clone();

  firstStep.init(std::move(pipeline_data), std::move(io));
  intermediateStep.init(std::move(reduction_data_2nd), std::move(io2));
  altStep.init(std::move(reduction_data_alt), std::move(io_alt));
}

/* Could be rewritten to be fully automated.
 * @numVectors: expect global size
 */
void MapReducePipeline::run(GLuint numVectors) {
  if (numVectors > 1024) {
    // first reduction run
    BenchmarkerGPU::getInstance().time(
        "MapReducePipeline Map+Reduce", [this, numVectors]() {
          GLuint buffer_size_before = numVectors;
          GLuint global_loads_per_thread =
              buffer_size_before / local_size.x / 8;
          if (!IsPowerOfTwo(buffer_size_before)) {
            global_loads_per_thread = next_pow_2(global_loads_per_thread);
          }
          /* Due to tests we aim at a grid/dispatch size between 8-16 for the
           * 2nd run, this would need some better statistics however and was
           * heuristically determined.
           *
           * Thus we the amount of sequential binary operations we go reduces to
           * 8-16.
           */
          buffer_size_after =
              buffer_size_before / local_size.x / global_loads_per_thread;

          firstStep.dispatch_with_barrier(
              {buffer_size_before, buffer_size_after});
        });
    std::string log_name = "MapReducePipeline Reduce: (" +
                           std::to_string(buffer_size_after) + " -> 1)";
    // 2nd reduction run
    BenchmarkerGPU::getInstance().time(log_name, [this]() {
      GLuint buffer_size_before = buffer_size_after;
      // only one invocation
      buffer_size_after = 1;
      intermediateStep.dispatch_with_barrier(
          {buffer_size_before, buffer_size_after});
    });
  } else {  // altered reduction if size is below 1024
    std::string log_name =
        "MapReducePipeline altStep: (" + std::to_string(numVectors) + " -> 1)";
    BenchmarkerGPU::getInstance().time(log_name, [this, &numVectors]() {
      GLuint buffer_size_before = numVectors;
      buffer_size_after = 1;
      altStep.dispatch_with_barrier({buffer_size_before, buffer_size_after});
    });
  }
}
/* Runs map reduce without sequential binary operations per thread.
 */
void MapReducePipeline::runNoSeqAdd(GLuint numVectors) {
  buffer_size_after = numVectors / local_size.x;
  BenchmarkerGPU::getInstance().time(
      "MapReducePipeline Map+Reduce",
      [this, numVectors]() { firstStep.dispatch_with_barrier(numVectors); });
}

