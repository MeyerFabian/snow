//multi buffer
#ifndef ABSTRACT_LAYOUT
#define AOS(cont, var, size, ind, buf_num, buf_ind) cont[buf_num*buf_ind+ind].var
#define SOA(cont, var, size, ind, buf_num, buf_ind) cont[buf_num*size*var+buf_ind*size+ind]
#else
#define AOS(cont, var, size, ind, buf_num, buf_ind,var_size) cont[(buf_num*buf_ind+ind)*var_size+var]
#define SOA(cont, var, size, ind, buf_num, buf_ind,var_size) cont[buf_num*size*var+buf_ind*size+ind]
#endif
