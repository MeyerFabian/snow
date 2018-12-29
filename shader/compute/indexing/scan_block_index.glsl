#ifndef GET_SCAN_INDEX
#define GET_SCAN_INDEX
//returns uvec2(scan_local_index, scan_block_index)
uint get_scan_local_index(uint voxel_and_tile_index){
	return voxel_and_tile_index;
}

#ifndef scanBlockSize
uniform uint scanBlockSize;
#endif
uint get_scan_block_index(uint voxel_and_tile_index){
	return voxel_and_tile_index / scanBlockSize;
}
#endif
