#ifndef P2GSHAREDATOMICCOMPUTE_H
#define P2GSHAREDATOMICCOMPUTE_H
#include <memory>

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../../shader/shared_hpp/voxel_block_size.hpp"
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/defines.hpp"
#include "../IOBufferData.hpp"
class P2G_shared_atomic : public Technique {
 public:
  LocalSize local_size = {VOXEL_DIM_X, VOXEL_DIM_Y, VOXEL_DIM_Z};
  struct UniformsStatic {
    glm::uvec3 gGridDim;
  };

  struct P2GData {
    UniformsStatic uniforms;
  };

  struct P2GBatchingData {
    UniformsStatic uniforms;
    GLuint multiple_particles;
  };

  struct UniformsDynamic {};
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_atomic(P2GData&& data, IOBufferData&& io);
  void init_loop_reverse(P2GData&& data, IOBufferData&& io);
  void init_batching(P2GBatchingData&& data, IOBufferData&& io);

 private:
  void init(P2GData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  glm::uvec3 gGridDim;
  std::string filename;
  std::vector<Shader::CommandType> vec = {};
};
#endif  // P2GSHAREDATOMICCOMPUTE_H

