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
  LocalSize local_size = {32, 1, 1};

  struct ScanData {
    std::string gl_unary_op_return_type;
    std::string gl_unary_op;
    std::string gl_binary_op_neutral_elem;
    std::string gl_binary_op;
    IOBufferData io;
    GLuint numVectors;  // SOA only
  };

  void init(ScanData&& data);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

