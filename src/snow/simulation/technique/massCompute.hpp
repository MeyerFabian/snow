#ifndef MASSCOMPUTE_H
#define MASSCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class MassCompute : public Technique {
 public:
  void init(string cs);
};
#endif  // MASSCOMPUTE_H

