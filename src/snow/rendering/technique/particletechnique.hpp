#ifndef PARTICLETECHNIQUE_H
#define PARTICLETECHNIQUE_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class ParticleTechnique : public Technique {
 public:
  void init(string vs, string fs);
};

#endif  // PARTICLETECHNIQUE_H

