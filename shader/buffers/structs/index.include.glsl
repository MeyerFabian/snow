#ifndef PARTICLE_INDEX_GLSL
#define PARTICLE_INDEX_GLSL

#ifdef AOS_LAYOUT
struct Indices {
	GLuint Index_i;
};
#else
#define Indices GLuint
#define Index_i         0
#define Indices_VAR_SIZE 1
#endif

#endif /* end of include guard: PARTICLE_INDEX_GLSL */
