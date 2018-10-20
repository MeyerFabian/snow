#version 440

// Automizing this shader is possible with more reflection information and abstracting aos layout to a primitive type
// e.g.
//
// buffer particle_buffer{
// vec4 particles[];
// }
// where the distinct variables lie directly after each other
// [pos,vel,pos,vel,pos,vel,...,pos,vel]
//
//
// INPUT    offsets		  [N]
// INPUT2   scan on local level	  [M]
// INPUT3   scan on block level	  [M]
// INPUT3+X container to sort     [N] ->| Double-
// OUTPUTX  container to put sort [N] ->| Buffer prob.
// INPUT4_VAR needs position variable



layout(local_size_x =X)in;


uniform uvec3 gGridDim;
uniform PREC_VEC3_TYPE gGridPos;
uniform PREC_SCAL_TYPE gridSpacing;

uniform uint bufferSize;

void main(void){
	uint tIndex = gl_GlobalInvocationID.x;
	if(i>=bufferSize){
		return;
	}

	PREC_VEC3_TYPE pos = INPUT4_AT(INPUT4,INPUT4_VAR,INPUT4_SIZE,i,INPUT4_NUM_BUFFER,INPUT4_INDEX_BUFFER,INPUT4_VAR_SIZE).xyz;

	// Bin due to position in grid
	PREC_VEC3_TYPE positionInGrid= (pos-gGridPos)/gridSpacing;

	//floor
	ivec3 globalGridIndex = ivec3(positionInGrid);

	if(inBounds(globalGridIndex,gGridDim)){

		uint voxelAndTileIndex = get_voxel_and_tile_index(globalGridIndex,gGridDim);
		uint newSortedIndex =
			//scan_local
			INPUT2_AT(INPUT2,INPUT2_VAR,INPUT2_SIZE,get_scan_local_index(voxelAndTileIndex),INPUT2_NUM_BUFFER,INPUT2_INDEX_BUFFER,INPUT2_VAR_SIZE) +
			//scan_block
			INPUT3_AT(INPUT3,INPUT3_VAR,INPUT3_SIZE,get_scan_block_index(voxelAndTileIndex),INPUT3_NUM_BUFFER,INPUT3_INDEX_BUFFER,INPUT3_VAR_SIZE);

	}
}
