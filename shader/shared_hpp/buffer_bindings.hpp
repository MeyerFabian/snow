#ifndef BUFFER_BINDINGS_H
#define BUFFER_BINDINGS_H
// at some point one would need to come up with a per shader binding structure
// ssbos
// particle buffers
#define PARTICLE_SYSTEM_BINDING 0
#define PARTICLE_SYSTEM_BINDING_2 1
#define PARTICLE_INDICES_BINDING 2
#define PARTICLE_GRIDOFFSET_BINDING 3
// grid buffers
#define ATOMIC_COUNTER_BINDING 4
#define SCAN_BUFFER_BINDING 5
#define UNIFORM_GRID_BINDING 6
// grid blocks buffer
#define BLOCKS_BINDING 7
#define BLOCKS_INDIRECT_BINDING 8

// ubos
// particle double buffer offset
#define UNIFORM_SORTED_BINDING 0
#define GRID_DEFINES_BINDING 1
#endif

