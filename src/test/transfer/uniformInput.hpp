#ifndef INPUT
#define INPUT

#include <glm/gtc/random.hpp>
#include "../../core/object/grid/grid_def.hpp"
#include "../../core/object/grid/gridpoint_exp.hpp"
#include "../../core/object/particle/particle_exp.hpp"
#include "testData.hpp"
static testData createInput() {
  GLuint numParticles = 100 * 100 * 100;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);

  for (size_t x = 0; x < 100; x++) {
    for (size_t y = 0; y < 100; y++) {
      for (size_t z = 0; z < 100; z++) {
        particles2.push_back({{0.0f, 0.0f, 0.0f, 0.0f}});
        // 0
        particles.push_back(
            {// pos_vol keep in grid +support bounds
             PREC_VEC_TYPE(2.5 * gridSpacing + float(x) * gridSpacing,
                           2.5 * gridSpacing + float(y) * gridSpacing,
                           2.5 * gridSpacing + float(z) * gridSpacing, 0.0f),
             // vel_mass
             PREC_VEC_TYPE(glm::linearRand(-1.0f, 1.0f),
                           glm::linearRand(-1.0f, 1.0f),
                           glm::linearRand(-1.0f, 1.0f), 1.0f)});
      }
    }
  }

#ifdef FULL_SORTED
  // double buffer
  for (size_t i = 0; i < numParticles; i++) {
    particles2.push_back({{}});
    // 0
    particles.push_back({{}});
  }
#endif
  return {
      numParticles, numGridPoints, particles, particles2,
      gGridPos,     gridSpacing,   gGridDim,
  };
}
#endif

