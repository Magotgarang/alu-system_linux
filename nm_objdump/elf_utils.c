#include "elf_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <elf.h>

/**
 * print_symbol_table - Prints the ELF symbol table
 * @symtab: Pointer to symbol table
 * @count: Number of symbols
 * @strtab: Pointer to string table
 */
void print_symbol_table(const Elf64_Sym *symtab, size_t count, const char *strtab)
{
    for (size_t i = 0; i < count; i++)
    {
        const Elf64_Sym *sym = &symtab[i];
        const char *name = &strtab[sym->st_name];
        char type = get_symbol_type(sym);

        printf("%016lx %c %s\n", sym->st_value, type, name);
    }
}

/**
 * parse_elf_file - Parses an ELF file and prints symbols
 * @filename: Name of the ELF file to parse
 *
 * Return: 0 on success, 1 on failure
 */
int parse_elf_file(const char *filename)
{
    int fd;
    struct stat st;
    void *map;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (1);
    }

    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        close(fd);
        return (1);
    }

    map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return (1);
    }

    process_elf(map);

    munmap(map, st.st_size);
    close(fd);
    return (0);
}

/**
 * get_symbol_type - Returns the symbol type
 * @sym: Pointer to ELF symbol
 *
 * Return: A character representing the symbol type
 */
char get_symbol_type(const Elf64_Sym *sym)
{
    switch (ELF64_ST_TYPE(sym->st_info))
    {
    case STT_NOTYPE:
        return 'U';
    case STT_OBJECT:
        return 'O';
    case STT_FUNC:
        return 'T';
    case STT_SECTION:
        return 'S';
    case STT_FILE:
        return 'F';
    default:
        return '?';
    }
}

