struct Input{
	dvec4 v;
};
struct Output{
	double f;
};

layout(std430, binding = 1) buffer vectors{
	Input g_in[ ];
};
layout(std430, binding = 2) buffer scalars{
	Output g_out[ ];
};
