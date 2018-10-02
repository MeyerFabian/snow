#ifndef COUNTER_GLSL
#define COUNTER_GLSL

#ifdef AOS_LAYOUT
struct Counter {
  uint Counter_i;
};
#else
#define Counter uint
#define Counter_i 		0
#endif

#endif /* end of include guard: PARTICLE_GLSL */

