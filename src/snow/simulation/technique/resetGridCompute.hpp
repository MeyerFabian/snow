#ifndef RESETGRID_H
#define RESETGRID_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
class ResetGridCompute : public Technique {
 public:
  void init(string cs);
};

#endif  // RESETGRID_H

