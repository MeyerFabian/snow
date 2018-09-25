#include "shader/test/soa_aos/gpu_in_out_structs.glsl"
layout(std430, binding = 1) buffer vectors{
	BUFFER_LAYOUT(Input,g_in);
};
layout(std430, binding = 2) buffer scalars{
	BUFFER_LAYOUT(Output,g_out);
};
