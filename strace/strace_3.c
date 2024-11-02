#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <syscall.h>
#include <errno.h>
#include <fcntl.h>

struct syscall_entry {
    const char *name;
    int nb_params;
};

struct syscall_entry syscalls_64_g[] = {
    {"read", 3},
    {"write", 3},
    {"open", 2},
    {"close", 1},
    {"mmap", 6},
    {"munmap", 2},
    {"brk", 1},
    {"execve", 3},
    {"exit_group", 1},
    {"access", 2},
    // Add more syscalls as needed...
};

void print_params(long params[], int nb_params) {
    for (int i = 0; i < nb_params; i++) {
        if (i > 0) printf(", ");
        if (i < 6) {  // Assuming we are only dealing with the first 6 params for simplicity
            printf("0x%lx", params[i]);
        } else {
            printf("...");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t child;
    child = fork();
    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return EXIT_FAILURE;
    } else if (child < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    int status;
    struct user_regs_struct regs;

    while (1) {
        wait(&status);
        if (WIFEXITED(status)) break;

        ptrace(PTRACE_GETREGS, child, NULL, &regs);

        // Get syscall number
        long syscall_num = regs.orig_rax;
        if (syscall_num < 0 || syscall_num >= sizeof(syscalls_64_g)/sizeof(syscalls_64_g[0])) {
            printf("syscall(%ld) = ?\n", syscall_num);
            continue;
        }

        struct syscall_entry *entry = &syscalls_64_g[syscall_num];
        long params[6] = {
            regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9
        };

        // Print syscall name and parameters
        printf("%s(", entry->name);
        print_params(params, entry->nb_params);
        printf(")");

        // Execute the syscall
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        wait(&status);
        ptrace(PTRACE_GETREGS, child, NULL, &regs);

        // Get return value
        long ret_val = regs.rax;
        printf(" = 0x%lx\n", ret_val);
    }

    return EXIT_SUCCESS;
}
