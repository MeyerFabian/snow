// #replace atomicAdd(mem,data)
#ifdef GL_NV_shader_atomic_float
atomicAdd(mem,data);
#else
// For non-Nvidia hardware
// OpenGLInsights p.310 Listing 22.1
uint newVal = floatBitsToUint(data);
uint prevVal =0; uint curVal;

/*
 * 1) Try swapping once assuming underlying value in memory location is 0:
 *    0 is neutral element of addition => no addition necessary.
 *    => succeed? finished
 *    => fail? => 2)
 * 2) prevVal has the value currently in memory location. Add our data to
 *    the returned value and try swapping again.
 *    => succeed? finished
 *    => fail? Memory has been overwritten by other thread. => Do 2) again
 *    prevVal will be updated.
 */
while( (curVal = atomicCompSwap(mem,prevVal,newVal))!= prevVal){
	prevVal = curVal;
	newVal = floatBitsToUint((data + uintBitsToFloat(curVal)));
#endif //GL_NV_shader_atomic_float
}
