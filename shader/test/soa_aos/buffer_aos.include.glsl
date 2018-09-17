struct Input{
	PREC_VEC_TYPE x;
	PREC_VEC_TYPE v;
};
struct Output{
	PREC_SCAL_TYPE f;
	PREC_SCAL_TYPE g;
};

layout(std430, binding = 1) buffer vectors{
	Input g_in[ ];
};
layout(std430, binding = 2) buffer scalars{
	Output g_out[ ];
};
