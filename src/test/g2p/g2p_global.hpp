#ifndef G2PGLOBALCOMPUTE_H
#define G2PGLOBALCOMPUTE_H
#include <memory>

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/defines.hpp"
#include "../IOBufferData.hpp"
class G2P_global : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct UniformsStatic {};
  struct G2PData {
    UniformsStatic uniforms;
  };

  struct UniformsDynamic {
    GLuint numParticles;
  };
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_looping(G2PData&& data, IOBufferData&& io);

 private:
  void init(G2PData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::string filename;
};
#endif  // G2PCOMPUTE_H

