#ifndef PARTICLE_HPP
#define PARTICLE_HPP
// shared data structure (C++/GLSL), which is enabled in c++
#include "../../snow/utils/defines.hpp"

#include "../../../../shader/shared_hpp/precision.hpp"

#ifndef AOS_LAYOUT
#define AOS_LAYOUT
#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/buffers/particle.include.glsl"

#undef AOS_LAYOUT
#else
#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/buffers/particle.include.glsl"
#endif

#endif /* end of include guard: GPU_IN_OUT_STRUCTS_HPP */

