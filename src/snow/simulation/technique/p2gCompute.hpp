#ifndef P2GCOMPUTE_H
#define P2GCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class P2GCompute : public Technique {
 public:
  void init(string cs);
};
#endif  // P2GCOMPUTE_H

