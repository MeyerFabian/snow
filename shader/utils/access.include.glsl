#ifdef AOS_LAYOUT
//multi buffer
#define AT(cont, var, size, ind, buf_num, buf_ind) cont[buf_num*buf_ind+ind].var
#else
#define AT(cont, var, size, ind, buf_num, buf_ind) cont[buf_num*size*var+buf_ind*size+ind]
#endif

