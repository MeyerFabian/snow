#ifndef PARTICLETECHNIQUE_H
#define PARTICLETECHNIQUE_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class ParticleTechnique : public Technique {
 public:
  ParticleTechnique();
  bool init(std::string vs, std::string fs);
  void setWVP(const Matrix4f*);

 private:
  GLuint ModelViewPersp;
};

#endif  // PARTICLETECHNIQUE_H

