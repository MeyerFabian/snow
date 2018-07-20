#ifndef PARTICLE_H
#define PARTICLE_H
#include "math3d.hpp"
struct Particle {
  Particle(Vector3f r = Vector3f(0.0f, 0.0f, 0.0f),
           Vector3i v = Vector3i(0, 0, 0), float m = 1.25e-3f, float V = 1e-9,
           Matrix4f Fe = Matrix4f(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f),
           Matrix4f Fp = Matrix4f(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f))
      : position(r),
        velocity(v),
        mass(m),
        volume(V),
        forceElastic(Fe),
        forcePlastic(Fp) {}
  Particle(float x = .0f, float y = .0f, float z = .0f,
           Vector3i v = Vector3i(0, 0, 0), float m = 1.25e-3f, float V = 1e-9,
           Matrix4f Fe = Matrix4f(1.0f), Matrix4f Fp = Matrix4f(1.0f))
      : position(x, y, z),
        velocity(v),
        mass(m),
        volume(V),
        forceElastic(Fe),
        forcePlastic(Fp) {}
  Vector3f position;
  Vector3i velocity;

  float mass;
  int volume;

  Matrix4f forceElastic;
  Matrix4f forcePlastic;
};

#endif  // PARTICLE_H
