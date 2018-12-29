#ifndef STREAMCOMPACTIONTECHNIQUE_H
#define STREAMCOMPACTIONTECHNIQUE_H
#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class StreamCompactionTechnique : public Technique {
 public:
  LocalSize local_size;

  struct StreamCompactionData {
    LocalSize local_size;
    std::string gl_unary_op;
  };

  struct DispatchData {
    GLuint bufferSize;
  };

  struct UniformsStatic {};

  struct UniformsDynamic {
    GLuint bufferSize;
  };

  void init(StreamCompactionData&& data, IOBufferData&& io);
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(UniformsDynamic&& uniforms) const;

  void uniforms_init(UniformsStatic&& uniforms) const;
};
#endif

