#ifndef ELF_UTILS_H
#define ELF_UTILS_H

#include <elf.h>

void print_symbol_table(const Elf64_Sym *symtab, size_t count, const char *strtab);
int parse_elf_file(const char *filename);
char get_symbol_type(const Elf64_Sym *sym);
void process_elf(void *map);

#endif /* ELF_UTILS_H */
