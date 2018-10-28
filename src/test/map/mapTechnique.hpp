#ifndef MAPTECHNIQUE_H
#define MAPTECHNIQUE_H
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class MapTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};
  bool multiple;
  struct MapData {
    std::string filename;
    std::string gl_unary_op;
    GLuint numVectors;  // SOA only
  };

  struct DispatchData {
    GLuint bufferSize;
    bool multiple = false;
    GLuint seq_elements = 1;
  };

  struct UniformsDynamic {
    GLuint bufferSize;
    GLuint dispatchDim_x;
  };

  void uniform_update(UniformsDynamic&& data);
  void init(MapData&& data, IOBufferData&&);
  void dispatch(DispatchData&& data);
  void dispatch_with_barrier(DispatchData&& data);
};
#endif

