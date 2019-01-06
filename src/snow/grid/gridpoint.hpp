#ifndef GRIDPOINT_HPP
#define GRIDPOINT_HPP
// shared data structure (C++/GLSL), which is enabled in c++
//

#include "../../snow/utils/defines.hpp"

#include "../../../shader/shared_hpp/precision.hpp"

#ifndef AOS_LAYOUT
#define AOS_LAYOUT
#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/buffers/structs/grid.include.glsl"

#undef AOS_LAYOUT
#else
#include "../../../shader/utils/access.include.glsl"

#include "../../../shader/buffers/structs/grid.include.glsl"
#endif

#endif /* end of include guard: GRIDPOINT_HPP */

