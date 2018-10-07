#ifndef SCAN_HPP
#define SCAN_HPP
// shared data structure (C++/GLSL), which is enabled in c++
#include "../../../../shader/shared_hpp/precision.hpp"

#ifndef AOS_LAYOUT
#define AOS_LAYOUT

#include "../../../shader/buffers/structs/scan.include.glsl"

#undef AOS_LAYOUT
#else

#include "../../../shader/buffers/structs/scan.include.glsl"
#endif

#endif /* end of include guard: GPU_IN_OUT_STRUCTS_HPP */

