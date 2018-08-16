#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H
#include <string>
#include "../../shader/technique.hpp"
#include "glm.hpp"
#include "math3d.hpp"
using namespace std;
class LightingTechnique : public Technique {
 public:
  void init(string vs, string fs);
};

#endif  // LIGHTINGTECHNIQUE_H
