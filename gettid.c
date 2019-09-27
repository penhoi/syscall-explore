#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define gettid() syscall(SYS_gettid)

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

int main(int argc, char *argv[])
{
       pid_t pid, tid;

       pid = getpid();
       tid = gettid();

       YPH_PRINTF("PID: %d\n", pid);
       YPH_PRINTF("TID: %d\n", tid);

       return 0;
}
