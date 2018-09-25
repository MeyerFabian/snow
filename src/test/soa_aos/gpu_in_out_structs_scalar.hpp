#ifndef GPU_IN_OUT_STRUCTS_SCALAR_HPP
#define GPU_IN_OUT_STRUCTS_SCALAR_HPP
// shared data structure (C++/GLSL), which is enabled in c++
#ifndef AOS_LAYOUT
#define AOS_LAYOUT
#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/shared_hpp/precision.hpp"
#include "../../../shader/test/soa_aos/gpu_in_out_structs_scalar.glsl"

#undef AOS_LAYOUT
#else

#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/shared_hpp/precision.hpp"
#include "../../../shader/test/soa_aos/gpu_in_out_structs_scalar.glsl"
#endif

#endif /* end of include guard: GPU_IN_OUT_STRUCTS_HPP */

