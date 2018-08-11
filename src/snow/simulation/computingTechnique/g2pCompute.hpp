#ifndef G2PCOMPUTE_H
#define G2PCOMPUTE_H
#include "../../shader/technique.hpp"
class G2PCompute : public Technique {
 public:
  void init(std::string cs);
};
#endif  // G2PCOMPUTE_H

