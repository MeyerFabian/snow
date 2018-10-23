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
  bool dw_back;

  struct ReorderData {
    LocalSize local_size;
    std::string filename;
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
    bool dw_back;
    GLuint scan_block_size;
  };

  struct DispatchData {
    GLuint bufferSize;
  };

  struct UniformsStatic {
    GLuint scanBlockSize;
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };

  struct UniformsDynamic {
    GLuint bufferSize;
  };

  void init(ReorderData&& data, IOBufferData&& io);
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(UniformsDynamic&& uniforms) const;

  void uniforms_init(UniformsStatic&& uniforms) const;
};
#endif

