#ifndef VELGRIDCOMPUTE_H
#define VELGRIDCOMPUTE_H
#include "../../shader/technique.hpp"
class VelGridCompute : public Technique {
 public:
  void init(std::string cs);
};

#endif  // VELGRIDCOMPUTE_H

