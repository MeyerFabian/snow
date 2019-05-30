#ifndef G2PGLOBALCOMPUTE_H
#define G2PGLOBALCOMPUTE_H
#include <memory>

#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../shader/technique.hpp"
#include "../../../utils/defines.hpp"

/* Particles pull from every grid node in the support.
 * All grid node accesses are global memory loads.
 */
class G2P_global : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct G2PData {
    bool apic = false;
  };

  struct UniformsDynamic {
    GLuint numParticles;
  };
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_looping(G2PData&& data, IOBufferData&& io);

 private:
  void init(G2PData&& data, IOBufferData&& io);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::string filename;
};
#endif  // G2PCOMPUTE_H

