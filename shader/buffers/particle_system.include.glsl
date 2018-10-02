#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/particle.include.glsl"
layout(std430, binding = PARTICLE_SYSTEM_BINDING) buffer ParticleSystem{
	Particle_exp particles[];
};