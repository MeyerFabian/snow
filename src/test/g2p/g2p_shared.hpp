#ifndef G2PSHAREDATOMICCOMPUTE_H
#define G2PSHAREDATOMICCOMPUTE_H
#include <memory>

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include <optional>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../../shader/shared_hpp/voxel_block_size.hpp"
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/defines.hpp"
#include "../IOBufferData.hpp"
#include "../indirect_dispatch/IndirectDispatch.hpp"
class G2P_shared : public Technique {
 public:
  LocalSize local_size = {VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z, 1, 1};
  struct UniformsStatic {
    glm::uvec3 gGridDim;
  };

  struct G2PData {
    UniformsStatic uniforms;
    std::optional<std::shared_ptr<IndirectDispatch>> block_indirect;
    bool apic = false;
  };

  struct G2PBatchingData {
    GLuint multiple_particles;
    G2PData g2p_data;
  };

  struct UniformsDynamic {};
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);

  void init_pull(G2PData&& data, IOBufferData&& io);
  void init_pull_batching(G2PBatchingData&& data, IOBufferData&& io);

 private:
  void init(G2PData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::optional<std::shared_ptr<IndirectDispatch>> block_dispatch;
  glm::uvec3 gGridDim;
  std::string filename;
  std::vector<Shader::CommandType> vec = {};
};
#endif  // G2PSHAREDATOMICCOMPUTE_H

