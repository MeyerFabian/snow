#ifndef SCANTECHNIQUE_H
#define SCANTECHNIQUE_H
#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/buffer.hpp"
#include "../../../shader/technique.hpp"

/*
 * Executes an exclusive scan on a buffer.
 * e.g. binary_op = add and store partial results
 * |  6 |  4 |  3 |  2 |  1 |  5 |
 * |  0 |  6 | 10 | 13 | 15 | 16 |
 */
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

