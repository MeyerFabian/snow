#ifndef SCANWRITEBACKTECHNIQUE_H
#define SCANWRITEBACKTECHNIQUE_H
#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class ScanWriteBackTechnique : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct ScanWriteBackData {
    std::string gl_binary_op;
    GLuint numVectors;
    GLuint scanBlockSize;
  };

  void init(ScanWriteBackData&& data, IOBufferData&& io);
  void dispatch_with_barrier(GLuint numVectors) const;
};
#endif

