// reorder_is_sorted/reorder_keys is actually a little bit hacky,
// we use the infrastructure of indices to store the key used for testing
//
// you can actually test aos against soa layout (linearRand will give same
// result)
#include "reorder.hpp"
int main() {
  GLuint numParticles = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;
#ifdef REORDER_SINGLE
  std::vector<ParticleIndices> particle_indices;
#else
  std::vector<GLuint> particle_indices;
#endif
  // DOUBLE BUFFER
  for (size_t i = 0; i < numParticles; i++) {
    particles2.push_back({{0.0f, 0.0f, 0.0f, 0.0f}});
    // 0
    particles.push_back(
        {PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                       glm::linearRand(0.0f, 1.0f), 0.0f)});
#ifdef REORDER_SINGLE
    particle_indices.push_back({0, 0});
#else
    particle_indices.push_back({0});
#endif
  }
  for (size_t i = 0; i < numParticles; i++) {
    // 1
    particles.push_back({{}});
    particles2.push_back({{}});
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
      numParticles,     numGridPoints, particles, particles2, binning,
      particle_indices, scan,          gGridDim,  gGridPos,   gridSpacing,
  };
  auto OutputData = test(data);
  auto sorted_particles = OutputData.particles;
#ifdef REORDER_SINGLE
  auto sorted_particle_indices = OutputData.particle_indices;
  std::cout << std::endl;
  std::cout << "test output:" << std::endl;
  int n = 32;
  std::cout << "first " + std::to_string(n) + " keys " << std::endl;
  std::for_each_n(sorted_particle_indices.begin(), n, [](const auto& i) {
    std::cout << i.UnsortedIndex_i << ", ";
  });
  std::cout << std::endl;
#ifdef REORDER_KEYS

  std::cout << "particles sorted by grid indices (keys)? " << std::boolalpha
            << std::is_sorted(std::begin(sorted_particle_indices),
                              std::end(sorted_particle_indices),
                              [](const auto& lhs, const auto& rhs) {
                                return lhs.UnsortedIndex_i <
                                       rhs.UnsortedIndex_i;
                              })
            << '\n';
#endif
#endif
  return 0;
}

