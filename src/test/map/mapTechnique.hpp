#ifndef MAPTECHNIQUE_H
#define MAPTECHNIQUE_H
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class MapTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};

  struct MapData {
    std::string gl_unary_op;
    IOBufferData io;
    GLuint numVectors;  // SOA only
  };

  void init(MapData&& data);
  void dispatch(GLuint numVectors);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

