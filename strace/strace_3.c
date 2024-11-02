#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit, EXIT_FAILURE
#include <unistd.h>     // for fork, execve, STDERR_FILENO, pid_t
#include <errno.h>      // for errno
#include <string.h>     // for memset
#include <signal.h>     // for kill and SIGSTOP
#include <sys/wait.h>   // for waitpid
#include <sys/ptrace.h> // for ptrace
#include <sys/types.h>  // for pid_t type
#include <sys/user.h>   // for user_regs_struct

void trace_child(char **av);
void trace_parent(pid_t child_pid);
int await_syscall(pid_t child_pid);
unsigned long get_syscall_param(struct user_regs_struct uregs, size_t i);

// Add your syscall table, or link to it if it's in another file.
extern struct syscall_entry syscalls_64_g[]; // Array of syscall entries
struct syscall_entry {
    char *name;
    int nb_params;
    int params[6];
};

/**
 * main - Entry point for the program.
 * @argc: Argument count
 * @argv: Argument vector
 * Return: EXIT_SUCCESS or EXIT_FAILURE.
 */
int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s command [args...]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (child_pid == 0) {
        trace_child(argv);
    } else {
        trace_parent(child_pid);
    }

    return (EXIT_SUCCESS);
}

/**
 * trace_child - Sets up the child process for tracing.
 * @argv: Argument vector for execve.
 */
void trace_child(char **argv)
{
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    kill(getpid(), SIGSTOP);
    if (execve(argv[1], &argv[1], NULL) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

/**
 * trace_parent - Handles the tracing of system calls in the child process.
 * @child_pid: PID of the child process to trace.
 */
void trace_parent(pid_t child_pid)
{
    int status;
    struct user_regs_struct regs;

    waitpid(child_pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

    while (1) {
        if (await_syscall(child_pid) != 0) break;
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);

        // Retrieve and print syscall information
        int syscall_num = regs.orig_rax;
        struct syscall_entry *syscall = &syscalls_64_g[syscall_num];
        printf("%s(", syscall->name);

        for (int i = 0; i < syscall->nb_params; i++) {
            if (i > 0) printf(", ");
            if (syscall->params[i] == -1)  // VARARGS or unknown
                printf("...");
            else
                printf("0x%lx", get_syscall_param(regs, i));
        }

        if (await_syscall(child_pid) != 0) break;
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);

        // Print the return value of the syscall
        printf(") = 0x%lx\n", regs.rax);
    }
}

/**
 * await_syscall - Waits for the next system call in the traced child.
 * @child_pid: PID of the child process.
 * Return: 0 if syscall is intercepted, 1 if child has exited.
 */
int await_syscall(pid_t child_pid)
{
    int status;

    while (1) {
        ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
        waitpid(child_pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            return 0;
        }
        if (WIFEXITED(status)) {
            return 1;
        }
    }
}

/**
 * get_syscall_param - Gets a specific parameter for a syscall from registers.
 * @regs: Registers structure containing syscall parameters.
 * @index: Index of the parameter to retrieve.
 * Return: The value of the specified parameter.
 */
unsigned long get_syscall_param(struct user_regs_struct regs, size_t index)
{
    switch (index) {
        case 0: return regs.rdi;
        case 1: return regs.rsi;
        case 2: return regs.rdx;
        case 3: return regs.r10;
        case 4: return regs.r8;
        case 5: return regs.r9;
        default: return 0;
    }
}
