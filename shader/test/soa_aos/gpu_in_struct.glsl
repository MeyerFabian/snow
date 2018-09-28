#ifndef GPU_IN_STRUCTS_GLSL
#define GPU_IN_STRUCTS_GLSL

#ifdef AOS_LAYOUT
struct Input {
	PREC_VEC_TYPE in_x;
	PREC_VEC_TYPE in_v;
};
#else
#define Input PREC_VEC_TYPE
#define in_x 0
#define in_v 1
#endif

#endif /* end of include guard: GPU_IN_STRUCTS_GLSL */

