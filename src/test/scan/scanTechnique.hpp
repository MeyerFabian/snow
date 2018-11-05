#ifndef SCANTECHNIQUE_H
#define SCANTECHNIQUE_H
#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class ScanTechnique : public Technique {
 public:
  GLuint reduction_factor;
  LocalSize local_size;
  struct ScanData {
    LocalSize local_size;
    std::string filename;
    std::string gl_unary_op_return_type;
    std::string gl_unary_op;
    std::string gl_binary_op_neutral_elem;
    std::string gl_binary_op;
    GLuint numVectors;
    GLuint raking = 1;
  };

  struct DispatchData {
    GLuint bufferSize;
    GLuint dispatchDim_x;
  };

  void init(ScanData&& data, IOBufferData&& io);
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(DispatchData&& uniforms) const;
};
#endif
