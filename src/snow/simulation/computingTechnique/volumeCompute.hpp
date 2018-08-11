#ifndef VOLUMECOMPUTE_H
#define VOLUMECOMPUTE_H
#include "../../shader/technique.hpp"
class VolumeCompute : public Technique {
 public:
  void init(std::string cs);
};
#endif  // VOLUMECOMPUTE

