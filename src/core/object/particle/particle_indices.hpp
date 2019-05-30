#ifndef PARTICLE_INDICES_HPP
#define PARTICLE_INDICES_HPP
// shared data structure (C++/GLSL), which is enabled in c++
#include "../../utils/defines.hpp"

#include "../../../shader/shared_hpp/precision.hpp"

#ifndef AOS_LAYOUT
#define AOS_LAYOUT

#include "../../../shader/buffers/structs/particle_unsorted_index.include.glsl"

#undef AOS_LAYOUT
#else

#include "../../../shader/buffers/structs/particle_unsorted_index.include.glsl"
#endif

#endif /* end of include guard: PARTICLE_INDICES_HPP */

