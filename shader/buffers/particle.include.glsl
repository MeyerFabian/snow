#ifndef PARTICLE_GLSL
#define PARTICLE_GLSL

#ifdef AOS_LAYOUT
struct Particle_exp {
  PREC_VEC_TYPE Particle_pos_mass;
  PREC_VEC_TYPE Particle_vel_vol;
  
  PREC_VEC_TYPE Particle_ElasDeform_1;
  PREC_VEC_TYPE Particle_ElasDeform_2;
  PREC_VEC_TYPE Particle_ElasDeform_3;

  PREC_VEC_TYPE Particle_PlasDeform_1;
  PREC_VEC_TYPE Particle_PlasDeform_2;
  PREC_VEC_TYPE Particle_PlasDeform_3;

};
#else
#define Particle_exp PREC_VEC_TYPE
#define Particle_pos_mass 		0
#define Particle_vel_vol 		1

#define Particle_ElasDeform_1 	2
#define Particle_ElasDeform_2 	3
#define Particle_ElasDeform_3 	4

#define Particle_PlasDeform_1 	5
#define Particle_PlasDeform_2 	6
#define Particle_PlasDeform_3 	7
#endif

#endif /* end of include guard: PARTICLE_GLSL */

