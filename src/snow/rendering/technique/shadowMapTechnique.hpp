#ifndef SHADOW_MAP_TECHNIQUE_H
#define SHADOW_MAP_TECHNIQUE_H
#include "../../shader/technique.hpp"
class ShadowMapTechnique : public Technique {
 public:
  void init(std::string vs, std::string fs);
};

#endif  // SHADOW_MAP_TECHNIQUE_H

