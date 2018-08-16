#ifndef PARTICLECOMPUTE_H
#define PARTICLECOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
class ParticleCompute : public Technique {
 public:
  void init(string cs);
};

#endif  // PARTICLECOMPUTE_H

