/**
 * Takes an index with range [0,63] referring to one of the 64 neighbors
 * (4x*4y*4z = 64 Neighbors) and returns its relative signed and rounded position to
 * that grid node.
 */
int width = 4;
ivec3 windowOffset=ivec3(-1,-1,-1);

void getIJK(const  int index,inout ivec3 ijk){
	int temp = index%(width*width);
	ijk= ivec3(temp%width,temp/width,index/(width*width))+windowOffset;
}