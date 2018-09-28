#include "shader/test/soa_aos/gpu_in_struct_scalar.glsl"
layout(std430, binding = 1) buffer vectors{
	Input g_in[];
};