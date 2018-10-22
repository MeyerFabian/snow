#ifndef SCAN_GLSL
#define SCAN_GLSL

#ifdef AOS_LAYOUT
struct Scan {
	GLuint Scan_local_i;
	GLuint Scan_block_i;
};
#else
#define Scan GLuint
#define Scan_local_i	0
#define Scan_block_i	1
#define Scan_VAR_SIZE 2
#endif

#endif /* end of include guard: SCAN_GLSL */
