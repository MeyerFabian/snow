#ifndef GPU_OUT_STRUCTS_GLSL
#define GPU_OUT_STRUCTS_GLSL

#ifdef AOS_LAYOUT
struct Output {
	PREC_SCAL_TYPE out_f;
	PREC_SCAL_TYPE out_g;
};
#else
#define Output PREC_SCAL_TYPE
#define out_f 0
#define out_g 1
#endif

#endif /* end of include guard: GPU_OUT_STRUCTS_GLSL */
