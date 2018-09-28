#include "shader/test/soa_aos/gpu_out_struct_scalar.glsl"
layout(std430, binding = 2) buffer scalars{
	Output g_out[];
};
