#ifndef MAPTECHNIQUE_H
#define MAPTECHNIQUE_H
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"

class MapTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};

  struct MapData {
    std::string gl_unary_op;
    std::string input;
    std::string output;
    std::string input_var;
    std::string output_var;
    BufferInfo in_buffer;
    BufferInfo out_buffer;
    GLuint numVectors;  // SOA only
  };

  void init(MapData&& data);
  void dispatch_with_barrier(GLuint numVectors) {
    Technique::uniform_update("bufferSize", numVectors);
    glDispatchCompute(numVectors / local_size.x + 1, 1 / local_size.y,
                      1 / local_size.z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
};
#endif

