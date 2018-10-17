#ifdef AOS_LAYOUT
#define AT(cont, var, ind) cont[ind].var
#else
#define AT(cont, var, ind) cont[var*TOTAL_BUFFER_SIZE+ind]
#endif

