#ifndef P2GCOMPUTE_H
#define P2GCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class P2GCompute : public Technique {
 public:
  P2GCompute() = default;
  bool init(string cs);
  void setGridPos(const float gridPosx, const float gridPosy,
                  const float gridPosz);
  void setGridDim(const int gridDimx, const int gridDimy, const int gridDimz);
  void setGridSpacing(const float h);

  void setYoung();
  void setPoisson();
  void setHardening();
  void setCritComp();
  void setCritStretch();
  void setIndexSize(const int size);

 private:
  GLuint gGridPos;
  GLuint gGridDim;
  GLuint h;

  GLuint young;
  GLuint poisson;
  GLuint hardening;
  GLuint critComp;
  GLuint critStretch;
  GLuint indexSize;
};
#endif  // P2GCOMPUTE_H

