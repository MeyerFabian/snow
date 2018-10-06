#ifndef SCAN_GLSL
#define SCAN_GLSL

#ifdef AOS_LAYOUT
struct Scan {
  uint Scan_i;
};
#else
#define Scan uint
#define Scan_i		0
#endif

#endif /* end of include guard: SCAN_GLSL */
