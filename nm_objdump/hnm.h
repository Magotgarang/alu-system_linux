#ifndef HNM_H
#define HNM_H

#include <elf.h>

const char *get_symbol_type(unsigned char type);
void print_symbol_table(int file);

#endif /* HNM_H */
