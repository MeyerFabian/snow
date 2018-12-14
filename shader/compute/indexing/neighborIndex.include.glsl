/**
 * Takes an index with range [0,63] referring to one of the 64 neighbors
 * (4x*4y*4z = 64 Neighbors) and returns its relative signed and rounded position to
 * that grid node.
 */


ivec3 getIJK(const int index, int support){
	int width = support+support;
	ivec3 windowOffset = ivec3(-support+1,-support+1,-support+1);
	int temp = index%(width*width);
	return ivec3(temp%width,temp/width,index/(width*width))+windowOffset;
}

ivec3 getIJK(const int index, ivec3 dim){
	int temp = index%(dim.x*dim.y);
	return ivec3(temp%dim.x,temp/dim.x,index/(dim.x*dim.y));
}

