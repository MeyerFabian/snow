#ifdef AOS_LAYOUT
#define AT(cont, var, ind) cont[ind].var
#define BUFFER_LAYOUT(type,var) type var[]
#else
#define AT(cont, var, ind) cont.var[ind]
#define BUFFER_LAYOUT(type,var) type var
#endif

