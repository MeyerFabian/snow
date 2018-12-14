#include "atomic_global.hpp"
int main() {
  GLuint numParticles = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);

  for (size_t i = 0; i < numParticles; i++) {
    particles2.push_back({{0.0f, 0.0f, 0.0f, 0.0f}});
    // 0
    particles.push_back(
        {// pos_vol keep in grid +support bounds
         PREC_VEC_TYPE(glm::linearRand(0.0f + 2.0 * gridSpacing,
                                       1.0f - 2.0 * gridSpacing),
                       glm::linearRand(0.0f + 2.0 * gridSpacing,
                                       1.0f - 2.0 * gridSpacing),
                       glm::linearRand(0.0f + 2.0 * gridSpacing,
                                       1.0f - 2.0 * gridSpacing),
                       0.0f),
         // vel_mass
         PREC_VEC_TYPE(glm::linearRand(-1.0f, 1.0f),
                       glm::linearRand(-1.0f, 1.0f),
                       glm::linearRand(-1.0f, 1.0f), 1.0f)});
  }

#ifdef ATOMIC_FULL_SORTED
  // double buffer
  for (size_t i = 0; i < numParticles; i++) {
    particles2.push_back({{}});
    // 0
    particles.push_back({{}});
  }
#endif

  auto output = test({numParticles, numGridPoints, particles, particles2,
                      gGridDim, gGridPos, gridSpacing});

  auto mv_i = std::transform_reduce(
      std::begin(output.grid), std::end(output.grid),
      PREC_VEC_TYPE(0.0f, 0.0f, 0.0f, 0.0f), std::plus<>(),
      [](const auto& elem) { return elem.Gridpoint_vel_mass; });

  auto mv_p = std::transform_reduce(
      std::begin(particles), std::end(particles),
      PREC_VEC_TYPE(0.0f, 0.0f, 0.0f, 0.0f), std::plus<>(),
      [](const auto& elem) { return elem.Particle_vel_mass; });

  std::cout << "particles mass sum: " << mv_p.w << std::endl;
  std::cout << "grid mass sum: " << mv_i.w << std::endl;
  std::cout << "Difference: " << std::abs(mv_p.w - mv_i.w) << std::endl;

  std::cout << "particle momentum_x sum: " << mv_p.x << std::endl;
  std::cout << "grid momentum_x sum: " << mv_i.x << std::endl;
  std::cout << "Difference: " << std::abs(mv_p.x - mv_i.x) << std::endl;

  std::cout << "particle momentum_y sum: " << mv_p.y << std::endl;
  std::cout << "grid momentum_y sum: " << mv_i.y << std::endl;
  std::cout << "Difference: " << std::abs(mv_p.y - mv_i.y) << std::endl;

  std::cout << "particle momentum_z sum: " << mv_p.z << std::endl;
  std::cout << "grid momentum_z sum: " << mv_i.z << std::endl;
  std::cout << "Difference: " << std::abs(mv_p.z - mv_i.z) << std::endl;

  return 0;
}

