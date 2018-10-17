#ifndef SCAN_BUFFER
#define SCAN_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/scan.include.glsl"
#include "shader/indexing/scan_block_index.glsl"

layout(std430, binding = SCAN_BUFFER_BINDING) buffer ScanLocalBuffer{
  Scan scans[];
};
#endif
