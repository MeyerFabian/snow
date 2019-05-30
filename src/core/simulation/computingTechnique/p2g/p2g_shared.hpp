#ifndef P2GSHAREDCOMPUTE_H
#define P2GSHAREDCOMPUTE_H
#include <memory>

#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include <optional>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../../../shader/shared_hpp/voxel_block_size.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../indirect_dispatch/IndirectDispatch.hpp"
#include "../../../shader/technique.hpp"

/* Particle-to-Grid MPM-Transfer that uses shared memory.
 */
class P2G_shared : public Technique {
 public:
  LocalSize local_size = {VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z, 1, 1};
  struct UniformsStatic {
    glm::uvec3 gGridDim;
  };

  struct P2GData {
    UniformsStatic uniforms;
    std::optional<std::shared_ptr<IndirectDispatch>> block_indirect;
    bool apic = false;
  };

  struct P2GBatchingData {
    GLuint multiple_particles;
    P2GData p2g_data;
  };

  struct UniformsDynamic {};
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_atomic(P2GData&& data, IOBufferData&& io);
  void init_atomic_loop_reverse(P2GData&& data, IOBufferData&& io);
  void init_atomic_batching(P2GBatchingData&& data, IOBufferData&& io);

  void init_sync(P2GData&& data, IOBufferData&& io);
  void init_sync_batching(P2GBatchingData&& data, IOBufferData&& io);

  void init_pull_simple(P2GData&& data, IOBufferData&& io);
  void init_pull_multiple(P2GBatchingData&& data, IOBufferData&& io);

 private:
  void init(P2GData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::optional<std::shared_ptr<IndirectDispatch>> block_dispatch;
  glm::uvec3 gGridDim;
  std::string filename;
  std::vector<Shader::CommandType> vec = {};
};
#endif  // P2GSHAREDATOMICCOMPUTE_H

