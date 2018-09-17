//#define SOA
#ifdef SOA
#define AT(cont, var, ind) cont.var[ind]
#else
#define AT(cont, var, ind) cont[ind].var
#endif

