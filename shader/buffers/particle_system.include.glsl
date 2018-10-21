#ifndef PARTICLE_SYSTEM_BUFFER
#define PARTICLE_SYSTEM_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/particle.include.glsl"
layout(std430, binding = PARTICLE_SYSTEM_BINDING) buffer ParticleSystem{
	Particle_exp particles[];
};
layout(std430, binding = PARTICLE_SYSTEM_BINDING_2) buffer ParticleSystem_2{
	Particle_exp_2 particles_2[];
};
#endif
