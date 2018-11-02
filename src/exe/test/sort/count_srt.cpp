#include "count_srt.hpp"
int main() {
  GLuint numParticles = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;

  for (size_t i = 0; i < numParticles; i++) {
    particles2.push_back({{0.0f, 0.0f, 0.0f, 0.0f}});
    // 0
    particles.push_back(
        {PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                       glm::linearRand(0.0f, 1.0f), 0.0f)});
  }

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);

  auto output = test({numParticles, numGridPoints, particles, particles2,
                      gGridDim, gGridPos, gridSpacing});
  return 0;
}

