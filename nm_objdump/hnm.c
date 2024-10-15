#include <stdio.h>
#include "elf_utils.h"

/**
 * main - Entry point of the hnm program
 * @argc: Argument count
 * @argv: Argument vector (array of strings)
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [objfile ...]\n", argv[0]);
        return (1);
    }

    for (int i = 1; i < argc; i++)
    {
        if (parse_elf_file(argv[i]) != 0)
        {
            fprintf(stderr, "Error: Could not parse file %s\n", argv[i]);
            return (1);
        }
    }
    return (0);
}
