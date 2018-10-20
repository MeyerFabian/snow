#include "reorder.hpp"
int main() {
  GLuint numParticles = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;
  std::vector<GLuint> grid_offset;
  for (size_t i = 0; i < numParticles; i++) {
    particles.push_back(
        {PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                       glm::linearRand(0.0f, 1.0f), 0.0f)});
    particles2.push_back({{(float)i, 0.0f, 0.0f, 0.0f}});
    grid_offset.push_back({0});
  }

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);
  std::vector<GLuint> binning;
  std::vector<Scan> scan;
  for (size_t i = 0; i < numGridPoints; i++) {
    binning.push_back(0);
    scan.push_back({0, 0});
  }
  testData data = {
      numParticles, numGridPoints, particles, particles2, binning,
      grid_offset,  scan,          gGridDim,  gGridPos,   gridSpacing,
  };
  auto OutputData = test(data);
  auto sorted_particles = OutputData.particles;

  std::cout << "end" << std::endl;
  return 0;
}

