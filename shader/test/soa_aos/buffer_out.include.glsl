#include "shader/test/soa_aos/gpu_out_struct.glsl"
layout(std430, binding = 2) buffer scalar{
	Output g_out[];
};

