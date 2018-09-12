#ifndef MAPREDUCE_BUFFERS_H
#define MAPREDUCE_BUFFERS_H
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <memory>
#include <vector>
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
class MapReduceBuffers {
 public:
  struct Input {
    Input(glm::vec4 n_v) : v(n_v) {}
    glm::vec4 v;
  };

  struct Output {
    Output(float n_f) : f(n_f) {}

    float f;
  };
  MapReduceBuffers(GLuint numVectors, LocalSize local_size)
      : input(std::make_shared<Buffer<Input>>(BufferType::SSBO,
                                              BufferUsage::STATIC_DRAW)),
        output(std::make_shared<Buffer<Output>>(BufferType::SSBO,
                                                BufferUsage::STATIC_DRAW)) {
    for (GLuint i = 0; i < numVectors; i++) {
      input_data.emplace_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }
    for (GLuint i = 0; i < numVectors / local_size.x; i++) {
      output_data_init.emplace_back(0.0f);
    }

    input->transfer_to_gpu(input_data);
    input->gl_bind_base(1);

    output->transfer_to_gpu(output_data_init);
    output->gl_bind_base(2);
  };

  std::vector<Input> input_data;
  std::vector<Output> output_data_init;
  std::shared_ptr<Buffer<Input>> input;
  std::shared_ptr<Buffer<Output>> output;
};
#endif

