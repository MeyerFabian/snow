#ifndef PARTICLECOMPUTE_H
#define PARTICLECOMPUTE_H
#include "../../shader/technique.hpp"
class ParticleCompute : public Technique {
 public:
  void init(std::string cs);
};
#endif  // PARTICLECOMPUTE_H

