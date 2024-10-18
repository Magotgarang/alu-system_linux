#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include "hnm.h"

// Function to get the symbol type as a string
const char *get_symbol_type(unsigned char type) {
    switch (type) {
        case STT_NOTYPE: return "N"; // No type
        case STT_OBJECT: return "O"; // Data object
        case STT_FUNC: return "F";    // Function
        case STT_SECTION: return "S"; // Section
        case STT_FILE: return "F";    // Source file
        case STT_TLS: return "T";     // Thread-local data
        default: return "?";          // Unknown
    }
}

// Function to print the symbol table
void print_symbol_table(int file) { 
    Elf64_Ehdr ehdr;
    Elf64_Shdr *shdr;
    Elf64_Sym *symtab;
    char *strtab;
    struct stat sb;

    // Get file statistics
    if (fstat(file, &sb) < 0) {
        perror("fstat failed");
        exit(EXIT_FAILURE);
    }
    
    // Read ELF header
    if (read(file, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
        perror("Failed to read ELF header");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for section headers
    shdr = malloc(ehdr.e_shnum * sizeof(Elf64_Shdr));
    if (!shdr) {
        perror("Failed to allocate memory for section headers");
        exit(EXIT_FAILURE);
    }

    // Seek to the section headers and read them
    lseek(file, ehdr.e_shoff, SEEK_SET);
    if (read(file, shdr, ehdr.e_shnum * sizeof(Elf64_Shdr)) !=
        (ssize_t)(ehdr.e_shnum * sizeof(Elf64_Shdr))) {
        perror("Failed to read section headers");
        free(shdr);
        exit(EXIT_FAILURE);
    }

    // Locate the symbol table section
    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB) {
            symtab = malloc(shdr[i].sh_size);
            if (!symtab) {
                perror("Failed to allocate memory for symbol table");
                free(shdr);
                exit(EXIT_FAILURE);
            }
            lseek(file, shdr[i].sh_offset, SEEK_SET);
            read(file, symtab, shdr[i].sh_size);

            // Locate the string table section
            strtab = malloc(shdr[shdr[i].sh_link].sh_size);
            if (!strtab) {
                perror("Failed to allocate memory for string table");
                free(symtab);
                free(shdr);
                exit(EXIT_FAILURE);
            }
            lseek(file, shdr[shdr[i].sh_link].sh_offset, SEEK_SET);
            read(file, strtab, shdr[shdr[i].sh_link].sh_size);

            // Print the symbol table
            for (unsigned long j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym); j++) {
                printf("%016lx %s %s\n", symtab[j].st_value,
                       get_symbol_type(ELF64_ST_TYPE(symtab[j].st_info)),
                       strtab + symtab[j].st_name);
            }

            free(strtab);
            free(symtab);
            break; // Break after processing the first symbol table
        }
    }

    free(shdr);
}

// Main function
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the ELF file
    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Print the symbol table
    print_symbol_table(file);

    // Close the file
    close(file);
    return EXIT_SUCCESS;
}
