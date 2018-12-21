#ifndef P2GSHAREDSYNCCOMPUTE_H
#define P2GSHAREDSYNCCOMPUTE_H
#include <memory>

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../../shader/shared_hpp/voxel_tile_size.hpp"
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/defines.hpp"
#include "../IOBufferData.hpp"
class P2G_shared_sync : public Technique {
 public:
  LocalSize local_size = {VOXEL_DIM_X, VOXEL_DIM_Y, VOXEL_DIM_Z};
  struct UniformsStatic {
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };
  struct P2GData {
    UniformsStatic uniforms;
  };

  struct UniformsDynamic {
    GLuint max_count;
  };
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_sync(P2GData&& data, IOBufferData&& io);

 private:
  void init(P2GData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  glm::uvec3 gGridDim;
  std::string filename;
};
#endif  // P2GSHAREDSYNCCOMPUTE_H

