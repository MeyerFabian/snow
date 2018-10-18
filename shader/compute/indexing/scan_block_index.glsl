uniform uint scanBlockSize;

uint get_scan_index(uint voxel_and_tile_index,uint scan_size){
	return 0;
		//AT(scans,Scan_local_i,scan_size,voxel_and_tile_index % scanBlockSize,1,0) +    AT(scans,Scan_block_i,scan_size,voxel_and_tile_index / scanBlockSize,1,0);
}
