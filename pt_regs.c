#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>

#define __USE_GNU
#include <signal.h>
#include <ucontext.h>

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/* return back by modifying regs directly instead of invoking syscall sigreturn */
static void handle_sigill(int sig, siginfo_t *siginfo, void *context)
{
    ucontext_t *ucp = (ucontext_t *)context;
    struct pt_regs regs;

    YPH_PRINTF("The address of invalid instruciton is %p\n", siginfo->si_addr);
    YPH_PRINTF("sp = %llx\n", ucp->uc_mcontext.gregs[REG_RSP]);

    // ucp->uc_mcontext.gregs[REG_RIP] += 2;
    regs.r15 = ucp->uc_mcontext.gregs[REG_R15];
    regs.r14 = ucp->uc_mcontext.gregs[REG_R14];
    regs.r13 = ucp->uc_mcontext.gregs[REG_R13];
    regs.r12 = ucp->uc_mcontext.gregs[REG_R12];
    regs.rbp = ucp->uc_mcontext.gregs[REG_RBP];
    regs.rbx = ucp->uc_mcontext.gregs[REG_RBX];
    regs.r11 = ucp->uc_mcontext.gregs[REG_R11];
    regs.r10 = ucp->uc_mcontext.gregs[REG_R10];
    regs.r9 = ucp->uc_mcontext.gregs[REG_R9];
    regs.r8 = ucp->uc_mcontext.gregs[REG_R8];
    regs.rax = ucp->uc_mcontext.gregs[REG_RAX];
    regs.rcx = ucp->uc_mcontext.gregs[REG_RCX];
    regs.rdx = ucp->uc_mcontext.gregs[REG_RDX];
    regs.rsi = ucp->uc_mcontext.gregs[REG_RSI];
    regs.rdi = ucp->uc_mcontext.gregs[REG_RDI];
    regs.orig_rax = ucp->uc_mcontext.gregs[REG_RAX];
    regs.rip = ucp->uc_mcontext.gregs[REG_RIP];
    regs.cs = ucp->uc_mcontext.gregs[REG_RIP];
    regs.eflags = ucp->uc_mcontext.gregs[REG_EFL];
    regs.rsp = ucp->uc_mcontext.gregs[REG_RSP];
    regs.ss = ucp->uc_mcontext.gregs[REG_RSP];

    /* return back by modifying registers */
    __asm__ volatile(
            "mov %[REG], %%rsp\n\t"
            "pop %%r15\n\t"
            "pop %%r14\n\t"
            "pop %%r13\n\t"
            "pop %%r12\n\t"
            "pop %%rbp\n\t"
            "pop %%rbx\n\t"
            "pop %%r11\n\t"
            "pop %%r10\n\t"
            "pop %%r9\n\t"
            "pop %%r8\n\t"
            "pop %%rax\n\t"
            "pop %%rcx\n\t"
            "pop %%rdx\n\t"
            "pop %%rsi\n\t"
            "pop %%rdi\n\t"
            "add $8, %%rsp\n\t" // "pop %%orig_rax\n\t"
            "pop %%rax\n\t"     // "pop %%rip\n\t"
            "add $8, %%rsp\n\t" // "pop %%cs\n\t"
            "popf\n\t"          // "pop %%rflags\n\t"
            "pop %%rsp\n\t"
            ""                  // "pop %%ss\n\t"
            "add $2, %%rax\n\t" // ucp->uc_mcontext.gregs[REG_RIP] += 2
            "jmp *%%rax\n\t"
            :
            :[REG] "rm" (&regs)
            :);
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    /* Register signal handler */
    memset(&act, '\0', sizeof(act));

    /* Use the sa_sigaction field because the handles has two additional parameters */
    act.sa_sigaction = &handle_sigill;

    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    act.sa_flags = SA_SIGINFO | SA_ONSTACK;

    if (sigaction(SIGILL, &act, NULL) < 0)
    {
        perror("sigaction");
        return 1;
    }

    YPH_PRINTF("Before UD2\n");

    /* Trigger error */
    __asm__("ud2\n\t");

    YPH_PRINTF("After UD2\n");

    return 0;
}
