#ifdef AOS_LAYOUT
#define AT(cont, var, ind) cont[ind].var
#define BUFFER_LAYOUT(type,var) type var[]
#define STRUCT_LAYOUT(type,var,n) type var
#else
#define AT(cont, var, ind) cont[0].var[ind]
#define BUFFER_LAYOUT(type,var) type var[]
#define STRUCT_LAYOUT(type,var,n) type var[n]
#endif

