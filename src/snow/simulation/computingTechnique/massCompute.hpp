#ifndef MASSCOMPUTE_H
#define MASSCOMPUTE_H
#include "../../shader/technique.hpp"
class MassCompute : public Technique {
 public:
  void init(std::string cs);
};
#endif  // MASSCOMPUTE_H

