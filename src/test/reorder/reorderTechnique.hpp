#ifndef REORDERTECHNIQUE_H
#define REORDERTECHNIQUE_H
#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class ReorderTechnique : public Technique {
 public:
  GLuint reduction_factor;
  LocalSize local_size;
  struct ReorderData {
    LocalSize local_size;
    std::string filename;
    GLuint numVectors;
    GLuint raking = 1;
  };

  struct DispatchData {
    GLuint bufferSize;
    GLuint dispatchDim_x;
  };

  void init(ReorderData&& data, IOBufferData&& io);
  void dispatch_with_barrier(GLuint numVectors);
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(DispatchData&& uniforms) const;
};
#endif

