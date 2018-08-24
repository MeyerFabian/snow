/**
 * Takes an integer vector ijk and returns the respective buffer index.
 * i of [0,x-GridDimension]
 * j of[0,y-GridDimension]
 * k of [0, z-GridDimension]
 */
void getIndex(const ivec3 ijk,inout int index){
	index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
}