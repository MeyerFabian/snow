#ifndef TILE_GLSL
#define TILE_GLSL

#ifdef AOS_LAYOUT
struct Tile {
	GLuint Tile_counter;
	GLuint Tile_scan_local;
	GLuint Tile_scan_block;
	GLuint Tile_index;
};
#else
#define Tile GLuint
#define Tile_counter 0
#define Tile_scan_local 1
#define Tile_scan_block 2
#define Tile_index 3
#define Tile_VAR_SIZE 4
#endif

#endif /* end of include guard: TILE_GLSL */
