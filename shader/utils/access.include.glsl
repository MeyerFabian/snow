#ifdef AOS_LAYOUT
#define AT(cont, var, ind) cont[ind].var
#else
#define AT(cont, var, ind) cont[var*N+ind]
#endif

