#ifndef GPU_IN_STRUCT_SCALAR_GLSL
#define GPU_IN_STRUCT_SCALAR_GLSL

#ifdef AOS_LAYOUT
struct Input {
	PREC_SCAL_TYPE in_x;
	PREC_SCAL_TYPE in_v;
};
#else
#define Input PREC_SCAL_TYPE
#define in_x 0
#define in_v 1
#endif

#endif /* end of include guard: GPU_IN_STRUCT_SCALAR_GLSL */
