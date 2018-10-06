//#include "bin.hpp"
int main() {
  /*
  GLuint numVectors = 1024 * 1024;
  GLuint numGridPoints = 128 * 128 * 128;
  std::vector<Particle_exp> particles;
  for (size_t i = 0; i < numVectors; i++) {
    particles.push_back({

        PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                      glm::linearRand(0.0f, 1.0f), 0.0f)

    });
  }

  PREC_VEC3_TYPE gGridPos(0.0, 0.0, 0.0);
  glm::uvec3 gGridDim(128, 128, 128);
  PREC_SCAL_TYPE gridSpacing(1.0 / 128.0);
  std::vector<GLuint> counter;
  for (size_t i = 0; i < numGridPoints; i++) {
    counter.push_back(0);
  }
  testData data = {
      numVectors, numGridPoints, particles,   counter,
      gGridDim,   gGridPos,      gridSpacing,
  };
  auto m = test(data);

  auto sum_gpu =
      std::transform_reduce(std::begin(m), std::end(m), 0.0f, std::plus<>(),
                            [](const auto& elem) { return elem; });
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(std::size(particles) - sum_gpu)
            << std::endl;
            */
  return 0;
}

