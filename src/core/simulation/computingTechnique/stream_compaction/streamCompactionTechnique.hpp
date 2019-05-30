#ifndef STREAMCOMPACTIONTECHNIQUE_H
#define STREAMCOMPACTIONTECHNIQUE_H
#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/buffer.hpp"
#include "../../../shader/technique.hpp"

/*
 * Stream compacts buffer which is equal to a filter operation in Haskell/Rust
 * Thus the size of the buffer decreases or stays the same (if the filter leads
 * to identity).
 */
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

