#ifndef G2PCOMPUTE_H
#define G2PCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class G2PCompute : public Technique {
 public:
  void init(string cs);
};
#endif  // G2PCOMPUTE_H

