#ifndef VELGRIDCOMPUTE_H
#define VELGRIDCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
class VelGridCompute : public Technique {
 public:
  void init(string cs);
};

#endif  // VELGRIDCOMPUTE_H

