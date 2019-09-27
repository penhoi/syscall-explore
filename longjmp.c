#include <stdio.h>
#include <setjmp.h>

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

static jmp_buf buf;

void second(void)
{
    YPH_PRINTF("Before invoking longjmp\n"); // prints
    longjmp(buf, 1);        // jumps back to where setjmp
    YPH_PRINTF("After invoking longjmp\n"); //   was called - making setjmp now return 1
}

void first(void)
{
    YPH_PRINTF("Before invoking second\n"); // does not print
    second();
    YPH_PRINTF("After invoking second\n"); // does not print
}

int main()
{
    if (!setjmp(buf))
    {
        YPH_PRINTF("After invoking setjmp, TRUE branch\n"); // does not print
        first(); // when executed, setjmp returns 0
    }
    else
    {                         // when longjmp jumps back, setjmp returns 1
        YPH_PRINTF("After invoking setjmp, FALSE branch\n"); // does not print
    }

    YPH_PRINTF("End of main\n"); // prints
    return 0;
}
