#ifndef VOLUMECOMPUTE_H
#define VOLUMECOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class VolumeCompute : public Technique {
 public:
  void init(string cs);
};
#endif  // VOLUMECOMPUTE

