#ifndef PARTICLE_GRIDOFFSET_GLSL
#define PARTICLE_GRIDOFFSET__GLSL

#ifdef AOS_LAYOUT
struct Particle_GridOffset {
	GLuint GridOffset_i;
};
#else
#define Particle_GridOffset GLuint
#define GridOffset_i		0
#define Particle_GridOffset_VAR_SIZE  1
#endif

#endif /* end of include guard: PARTICLE_INDEX_GLSL */
