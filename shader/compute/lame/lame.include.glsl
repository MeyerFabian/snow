/*
 * E0 Young Modulus
 * r Poissons ratio
 * Es hardening coefficient
 * Jp determinant of FPp
 * mu0 = E0/(2*(1+r))
 * mu = mu0 * e(Es(1-Jp))
 * JP=1.1, 1. Initial Parameters: 140000 /(2*(1+0.2))*e(10*(1.0-1.1) = 21459.63
 * JP=0.9, 2. Initial Parameters: 140000 /(2*(1+0.2))*e(10*(1.0-0.9) = 158566.43
 */
float mu(const float JP){
	return young /(2.0f*(1.0f+poisson))* exp(hardening*(1.0f-JP));
}

/*
 * E0 Young Modulus
 * r Poissons ratio
 * Es hardening coefficient
 * Jp determinant of FPp
 * lambda0 = E0*r/((1+r)(1-2r))
 * lambda = lambda0 * e(Es(1-Jp))
 * JP = 1.1,Initial Parameters: 140000*0.2/((1.0+0.2)*(1.9-2.0*0.2))*e^(10*(1-1.1)) = 5722.56
 * JP = 0.9, Initial Parameters: 140000*0.2/((1.0+0.2)*(1.9-2.0*0.2))*e^(10*(1-0.9)) = 42284.38
 */
float lambda(const float JP){
	return young*poisson /((1.0f+poisson)*(1.0f-2.0f*poisson))* exp(hardening*(1.0f-JP));
}