#ifndef PARTICLETECHNIQUE_H
#define PARTICLETECHNIQUE_H
#include "../../shader/technique.hpp"
class ParticleTechnique : public Technique {
 public:
  void init(std::string vs, std::string fs);
};

#endif  // PARTICLETECHNIQUE_H

