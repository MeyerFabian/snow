#ifndef P2GATOMICCOMPUTE_H
#define P2GATOMICCOMPUTE_H
#include <memory>

#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../shader/technique.hpp"

/* Particle-to-Grid MPM Transfer with atomics which is the most naive solution.
 */
class P2G_atomic_global : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct UniformsStatic {};
  struct P2GData {
    UniformsStatic uniforms;
  };

  struct UniformsDynamic {
    GLuint numParticles;
  };
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_too_parallel(P2GData&& data, IOBufferData&& io);
  void init_looping(P2GData&& data, IOBufferData&& io);

 private:
  void init(P2GData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::string filename;
  GLuint ydim;
  bool too_parallel = false;
};
#endif  // P2GCOMPUTE_H

