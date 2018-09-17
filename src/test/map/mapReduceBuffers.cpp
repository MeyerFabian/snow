#include "mapReduceBuffers.hpp"
MapReduceBuffers::MapReduceBuffers(GLuint numVectors, LocalSize local_size)
    : input(std::make_shared<Buffer<Input>>(BufferType::SSBO,
                                            BufferUsage::STATIC_DRAW)),
      output(std::make_shared<Buffer<Output>>(BufferType::SSBO,
                                              BufferUsage::STATIC_DRAW)) {
  for (GLuint i = 0; i < numVectors; i++) {
    input_data.emplace_back(glm::vec4(glm::ballRand(1.0f), 0.0f));
  }
  for (GLuint i = 0; i < numVectors; i++) {
    output_data_init.emplace_back(0.0f);
  }

  input->transfer_to_gpu(input_data);
  input->gl_bind_base(1);

  output->transfer_to_gpu(output_data_init);
  output->gl_bind_base(2);
};

// entirely for testing out of bounds shader accesses
MapReduceBuffers::MapReduceBuffers(GLuint numVectors, LocalSize local_size,
                                   GLuint numRepl)
    : input(std::make_shared<Buffer<Input>>(BufferType::SSBO,
                                            BufferUsage::STATIC_DRAW)),
      output(std::make_shared<Buffer<Output>>(BufferType::SSBO,
                                              BufferUsage::STATIC_DRAW)) {
  for (GLuint i = 0; i < numRepl * numVectors; i++) {
    input_data.emplace_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
  }
  for (GLuint i = 0; i < numVectors; i++) {
    output_data_init.emplace_back(0.0f);
  }

  input->transfer_to_gpu(input_data);
  input->gl_bind_base(1);

  output->transfer_to_gpu(output_data_init);
  output->gl_bind_base(2);
};

