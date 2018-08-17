#ifndef RESETGRID_H
#define RESETGRID_H
#include "../../shader/technique.hpp"
class ResetGridCompute : public Technique {
 public:
  void init(std::string cs);
};

#endif  // RESETGRID_H

