#if defined(FULL_SORTED) || defined(INDEX_WRITE_SORTED) || \
    defined(INDEX_READ_SORTED)
#define SORTED
#endif
#if defined(INDEX_WRITE_SORTED) || defined(INDEX_READ_SORTED)
#define INDEX_SORTED
#endif
#if defined(SHARED_ATOMIC) || defined(PUSH_SYNC) || defined(SHARED_PULL)
#define P2G_SHARED
#endif
#if defined(PUSH_SYNC)
#define BLOCK_COMPACTION
#endif

#if defined(SHARED_PULL)
#define BLOCK_COMPACTION
#endif

#if defined(UNIFORM_INPUT)
#include "../../../../src/test/transfer/uniformInput.hpp"
#elif defined(UNIFORM_INPUT_2_PER_CELL)
#include "../../../../src/test/transfer/uniformInput2percell.hpp"
#elif defined(UNIFORM_INPUT_4_PER_CELL)
#include "../../../../src/test/transfer/uniformInput4percell.hpp"
#else
#include "../../../../src/test/transfer/randomInput.hpp"
#endif
#include "roundtrip.hpp"
int main() {
  auto input = createInput();

  auto gGridDim = input.grid_def.gGridDim;
  auto gridSpacing = input.grid_def.gridSpacing;
  auto output = test(input);

  auto mv_i = std::transform_reduce(
      std::begin(output.grid), std::end(output.grid),
      PREC_VEC_TYPE(0.0f, 0.0f, 0.0f, 0.0f), std::plus<>(),
      [](const auto& elem) {
        return PREC_VEC_TYPE(
            elem.Gridpoint_vel_mass.x * elem.Gridpoint_vel_mass.w,
            elem.Gridpoint_vel_mass.y * elem.Gridpoint_vel_mass.w,
            elem.Gridpoint_vel_mass.z * elem.Gridpoint_vel_mass.w,
            elem.Gridpoint_vel_mass.w);
      });

  auto mv_p = std::transform_reduce(
      std::begin(input.particles), std::end(input.particles),
      PREC_VEC_TYPE(0.0f, 0.0f, 0.0f, 0.0f), std::plus<>(),
      [](const auto& elem) { return elem.Particle_vel_mass; });

  auto L_i = PREC_VEC3_TYPE(0.0f);
  for (size_t z = 0; z < gGridDim.z; z++) {
    for (size_t y = 0; y < gGridDim.y; y++) {
      for (size_t x = 0; x < gGridDim.x; x++) {
        auto mom = output.grid[x + y * gGridDim.x + z * gGridDim.x * gGridDim.y]
                       .Gridpoint_vel_mass;
        L_i += glm::cross(
            PREC_VEC3_TYPE(mom.x * mom.w, mom.y * mom.w, mom.z * mom.w),
            PREC_VEC3_TYPE(x, y, z) * gridSpacing);
      }
    }
  }
  auto L_p = std::transform_reduce(
      std::begin(input.particles), std::end(input.particles),
      PREC_VEC3_TYPE(0.0f, 0.0f, 0.0f), std::plus<>(), [](const auto& elem) {
        return glm::cross(PREC_VEC3_TYPE(elem.Particle_vel_mass),
                          PREC_VEC3_TYPE(elem.Particle_pos_vol));
      });

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

  std::cout << "particle angular_x sum: " << L_p.x << std::endl;
  std::cout << "grid angular_x sum: " << L_i.x << std::endl;
  std::cout << "Difference: " << std::abs(L_p.x - L_i.x) << std::endl;

  std::cout << "particle angular_y sum: " << L_p.y << std::endl;
  std::cout << "grid angular_y sum: " << L_i.y << std::endl;
  std::cout << "Difference: " << std::abs(L_p.y - L_i.y) << std::endl;

  std::cout << "particle angular_z sum: " << L_p.z << std::endl;
  std::cout << "grid angular_z sum: " << L_i.z << std::endl;
  std::cout << "Difference: " << std::abs(L_p.z - L_i.z) << std::endl;

  return 0;
}

