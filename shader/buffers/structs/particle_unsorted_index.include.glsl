#ifndef PARTICLE_INDEX_GLSL
#define PARTICLE_INDEX_GLSL

#ifdef AOS_LAYOUT
struct ParticleIndices {
	GLuint GridOffset_i;
	GLuint UnsortedIndex_i;
};
#else
#define ParticleIndices GLuint
#define GridOffset_i		0
#define UnsortedIndex_i         1
#define ParticleIndices_VAR_SIZE 2
#endif

#endif /* end of include guard: PARTICLE_INDEX_GLSL */
