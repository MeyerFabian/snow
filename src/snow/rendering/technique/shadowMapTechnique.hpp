#ifndef SHADOW_MAP_TECHNIQUE_H
#define SHADOW_MAP_TECHNIQUE_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class ShadowMapTechnique : public Technique {
 public:
  void init(string vs, string fs);
};

#endif  // SHADOW_MAP_TECHNIQUE_H

