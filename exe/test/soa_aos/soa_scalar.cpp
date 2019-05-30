#ifndef UNARY_OPERATION_GL
#include <glm/gtc/random.hpp>
#include <numeric>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "../../../src/test/soa_aos/gpu_in_out_structs_scalar.hpp"
#define UNARY_OPERATION_GL "value"
#define UNARY_OPERATION(value) value
#define INIT_TYPE "PREC_SCAL_TYPE"
#define INIT_DATA glm::linearRand(0.0f, 1.0f)
#define BUFFER_IN_NAME "shader/test/soa_aos/buffer_in_scalar.include.glsl"
#define BUFFER_OUT_NAME "shader/test/soa_aos/buffer_out_scalar.include.glsl"
#endif

#include "soa.cpp"

