#include "bin.hpp"
int main() {
  GLuint numVectors = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  std::vector<GLuint> grid_offset;
  for (size_t i = 0; i < numVectors; i++) {
    particles.push_back({

        PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                      glm::linearRand(0.0f, 1.0f), 0.0f)

    });
#ifdef OUTPUT2
    grid_offset.push_back({0});
#endif
  }

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);
  std::vector<GLuint> binning;
  for (size_t i = 0; i < numGridPoints; i++) {
    binning.push_back(0);
  }
  testData data = {
      numVectors,  numGridPoints, particles, binning,
      grid_offset, gGridDim,      gGridPos,  gridSpacing,
  };
  auto OutputData = test(data);
  auto count_gpu = OutputData.binning;

  auto sum_count_gpu = std::transform_reduce(
      std::begin(count_gpu), std::end(count_gpu), 0, std::plus<>(),
      [](const auto& elem) { return elem; });
  std::cout << "Count on GPU: " << sum_count_gpu << std::endl;
  std::cout << "Difference: " << std::size(particles) - sum_count_gpu
            << std::endl;
#ifdef OUTPUT2
  auto m2 = OutputData.grid_offset;

  auto counter_gauss_sum_gpu =
      std::transform_reduce(std::begin(count_gpu), std::end(count_gpu),
                            (GLuint)0, std::plus<>(), [](const auto& n) {
                              if (n == 0) {
                                return (GLuint)0;
                              } else {
                                return (GLuint)(n * (n - 1) / 2);
                              }
                            });

  auto elem_sum_gpu =
      std::transform_reduce(std::begin(m2), std::end(m2), 0, std::plus<>(),
                            [](const auto& elem) { return elem; });
  std::cout << "Gauss sum on Grid Counter: " << counter_gauss_sum_gpu
            << std::endl;
  std::cout << "Elem sum on Particles Offset: " << elem_sum_gpu << std::endl;

  std::cout << "Difference: " << counter_gauss_sum_gpu - elem_sum_gpu
            << std::endl;
#endif
  return 0;
}

