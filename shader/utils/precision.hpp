#ifndef PREC_TYPE
#define PREC_TYPE
//#define DOUBLE_PREC
#ifdef DOUBLE_PREC
#define PREC_SCAL_TYPE double
#define PREC_VEC_TYPE dvec4
#define PREC_MAT_TYPE dmat4
#else  // float
#define PREC_SCAL_TYPE float
#define PREC_VEC_TYPE vec4
#define PREC_MAT_TYPE mat4
#endif  // DOUBLE_PREC
#endif  // PREC_TYPE

