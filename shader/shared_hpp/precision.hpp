#ifndef PREC_TYPE
#define PREC_TYPE
#ifdef DOUBLE_PREC
#ifdef GLSL
#define PREC_SCAL_TYPE double
#define PREC_VEC_TYPE dvec4
#define PREC_MAT_TYPE dmat4
#else   // c++
typedef double PREC_SCAL_TYPE;
typedef glm::dvec4 PREC_VEC_TYPE;
typedef glm::dmat4 PREC_MAT_TYPE;
#endif  // GLSL
#else   // float
#ifdef GLSL
#define PREC_SCAL_TYPE float
#define PREC_VEC_TYPE vec4
#define PREC_MAT_TYPE mat4
#else   // c++
typedef float PREC_SCAL_TYPE;
typedef glm::vec4 PREC_VEC_TYPE;
typedef glm::mat4 PREC_MAT_TYPE;
#endif  // GLSL
#endif  // DOUBLE_PREC
#endif  // PREC_TYPE

