#ifndef GRIDOFFSET_GLSL
#define GRIDOFFSET_GLSL

#ifdef AOS_LAYOUT
struct GridOffset {
	uint GridOffset_i;
};
#else
#define GridOffset uint
#define GridOffset_i		0
#endif

#endif /* end of include guard: GRIDOFFSET_GLSL */
