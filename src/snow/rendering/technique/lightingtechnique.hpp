#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H
#include "../../shader/technique.hpp"
class LightingTechnique : public Technique {
 public:
  void init(std::string vs, std::string fs);
};

#endif  // LIGHTINGTECHNIQUE_H

