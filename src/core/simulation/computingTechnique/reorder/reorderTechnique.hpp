#ifndef REORDERTECHNIQUE_H
#define REORDERTECHNIQUE_H
#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/buffer.hpp"
#include "../../../shader/technique.hpp"

/*
 * Reorders buffer according to permutation.
 */
class ReorderTechnique : public Technique {
 public:
  LocalSize local_size;
  bool dw_back;

  struct ReorderData {
    LocalSize local_size;
    std::string filename;
    bool dw_back;
    GLuint scan_block_size;
  };

  struct DispatchData {
    GLuint bufferSize;
  };

  struct UniformsStatic {
    GLuint scanBlockSize;
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

