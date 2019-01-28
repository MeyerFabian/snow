#ifndef PARTICLE_GLSL
#define PARTICLE_GLSL

#ifdef AOS_LAYOUT
struct Particle_exp {
	PREC_VEC_TYPE Particle_pos_vol;
	PREC_VEC_TYPE Particle_vel_mass;

	PREC_VEC_TYPE Particle_Bp_1;
	PREC_VEC_TYPE Particle_Bp_2;
	PREC_VEC_TYPE Particle_Bp_3;
};
struct Particle_exp_2{
	PREC_VEC_TYPE Particle_PlasDeform_1;
	PREC_VEC_TYPE Particle_PlasDeform_2;
	PREC_VEC_TYPE Particle_PlasDeform_3;
};
#else
#define Particle_exp PREC_VEC_TYPE
#define Particle_pos_vol	0
#define Particle_vel_mass	1

#define Particle_Bp_1		2
#define Particle_Bp_2		3
#define Particle_Bp_3		4
#define Particle_exp_size 5

#define Particle_exp_2 PREC_VEC_TYPE
#define Particle_PlasDeform_1	0
#define Particle_PlasDeform_2	1
#define Particle_PlasDeform_3	2
#define Particle_exp_2_size 3
#endif

#endif /* end of include guard: PARTICLE_GLSL */

