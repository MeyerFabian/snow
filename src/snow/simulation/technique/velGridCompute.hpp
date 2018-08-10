#ifndef VELGRIDCOMPUTE_H
#define VELGRIDCOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
class VelGridCompute : public Technique {
 public:
  VelGridCompute();
  bool init(string cs);
  void setDt(const float dt);
  void setCritComp();
  void setCritStretch();
  void setCollisionOffset();
  void setGridDim(const int gridDimx, const int gridDimy, const int gridDimz);
  void setnumColliders(const int numColliders);
  void setIndexSize(const int particleSize);

 private:
  GLuint dt;
  GLuint critComp;
  GLuint critStretch;
  GLuint collisionOffset;
  GLuint gGridDimension;
  GLuint gNumColliders;
  GLuint indexSize;
};

#endif  // VELGRIDCOMPUTE_H

