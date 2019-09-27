#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define __USE_GNU
#include <signal.h>
#include <ucontext.h>

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__,  ##__VA_ARGS__)


static void set_sigstack(void *signal_stack)
{
    stack_t ss = {
        .ss_size = SIGSTKSZ,
        .ss_sp = signal_stack,
    };
    sigaltstack(&ss, 0);
}

static void handle_sigill(int sig, siginfo_t *siginfo, void *context)
{
    ucontext_t *ucp = (ucontext_t *)context;

    YPH_PRINTF("The address of invalid instruciton is %p\n", siginfo->si_addr);
    YPH_PRINTF("sp = %llx\n", ucp->uc_mcontext.gregs[REG_RSP]);
    ucp->uc_mcontext.gregs[REG_RIP] += 2;
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    void *sigstack;

    /* create signal_stack */
    sigstack = mmap(NULL, SIGSTKSZ, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (sigstack == NULL)
    {
        perror("mmap");
        return 1;
    }
    else
    {
        YPH_PRINTF("Signal stack is @%p\n", sigstack);
    }

    //set_sigstack(sigstack);

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
