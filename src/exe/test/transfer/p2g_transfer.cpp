#if defined(FULL_SORTED) || defined(INDEX_WRITE_SORTED) || \
    defined(INDEX_READ_SORTED)
#define SORTED
#endif
#if defined(INDEX_WRITE_SORTED) || defined(INDEX_READ_SORTED)
#define INDEX_SORTED
#endif
#if defined(SHARED_ATOMIC) || defined(PUSH_SYNC) || defined(SHARED_PULL)
#define SHARED
#endif
#if defined(PUSH_SYNC)
#define BLOCK_COMPACTION
#endif

#if defined(SHARED_PULL)
#define BLOCK_COMPACTION
#endif

#if defined(UNIFORM_INPUT)
#include "uniformInput.hpp"
#elif defined(UNIFORM_INPUT_2_PER_CELL)
#include "uniformInput2percell.hpp"
#elif defined(UNIFORM_INPUT_4_PER_CELL)
#include "uniformInput4percell.hpp"
#else
#include "randomInput.hpp"
#endif
#include "p2g_transfer.hpp"
int main() {
  auto input = createInput();

  auto output = test(input);

  auto mv_i = std::transform_reduce(
      std::begin(output.grid), std::end(output.grid),
      PREC_VEC_TYPE(0.0f, 0.0f, 0.0f, 0.0f), std::plus<>(),
      [](const auto& elem) { return elem.Gridpoint_vel_mass; });

  auto mv_p = std::transform_reduce(
      std::begin(input.particles), std::end(input.particles),
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

