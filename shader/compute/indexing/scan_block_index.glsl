uniform uint scanBlockSize;

uint get_scan_index(uint voxel_and_tile_index){
  return
    AT(scans,Scan_local_i,voxel_and_tile_index % scanBlockSize) +    AT(scans,Scan_block_i,voxel_and_tile_index / scanBlockSize);
}
