#ifndef GRID_GLSL
#define GRID_GLSL

#ifdef AOS_LAYOUT
struct Gridpoint {
	PREC_VEC_TYPE Gridpoint_vel_mass;
	PREC_VEC_TYPE Gridpoint_force;
};
#else
#define Gridpoint PREC_VEC_TYPE
#define Gridpoint_vel_mass 0
#define Gridpoint_force 1
#define Gridpoint_size 2
#endif

#endif /* end of include guard: GRIDPOINT_GLSL */
