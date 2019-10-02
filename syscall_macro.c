#include <stdio.h>
#include <stdlib.h>



#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/types.h>

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

long local_syscall();

void syscall_macro()
{
    __asm__ volatile (
            "local_syscall:\n\t"
            "mov    %rdi,%rax\n\t"
            "mov    %rsi,%rdi\n\t"
            "mov    %rdx,%rsi\n\t"
            "mov    %rcx,%rdx\n\t"
            "mov    %r8,%r10\n\t"
            "mov    %r9,%r8\n\t"
            "mov    0x8(%rsp),%r9\n\t"
            "syscall \n\t"
            "retq"
            );

}

#define syscall local_syscall
int main(int argc, char *argv[])
{
    pid_t pid, tid;
    int i;

    for (i = 0; i< 2; i++) {
        pid = syscall(SYS_getpid);
        YPH_PRINTF("(%d) PID is: %d\n", i, pid);
    }

    tid = syscall(SYS_gettid);
    YPH_PRINTF("TID is: %d\n", tid);


    return 0;
}
