#ifndef PARTICLECOMPUTE_H
#define PARTICLECOMPUTE_H
#include "../../defines/defines.hpp"
#include "../../shader/technique.hpp"
class ParticleCompute : public Technique {
 public:
  ParticleCompute();
  bool init(std::string cs);
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

#endif  // PARTICLECOMPUTE_H

