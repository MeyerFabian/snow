#ifndef BLOCK_GLSL
#define BLOCK_GLSL

#ifdef AOS_LAYOUT
struct Block {
	GLuint Block_counter;
	GLuint Block_scan_local;
	GLuint Block_scan_block;
	GLuint Block_index;
};
#else
#define Block GLuint
#define Block_counter 0
#define Block_scan_local 1
#define Block_scan_block 2
#define Block_index 3
#define Block_VAR_SIZE 4
#endif

#endif /* end of include guard: BLOCK_GLSL */
