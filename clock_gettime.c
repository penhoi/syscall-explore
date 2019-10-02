#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define YPH_PRINTF(fmt, ...) printf("[%s:%d]->\t\t" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

float data = 0.0;

void simple_loop(int tick)
{
    int i, j;
    
    for (i = 0; i< tick; i++) {
        for (j = 0; j < tick; j++) {
            data += j * 1.0 /( i + 0.1);
        }
    }
}

int main(int argc, char* argv[])
{
    struct timespec res1, res2;

    clock_gettime(CLOCK_REALTIME, &res1);
    
    simple_loop(10000);

    clock_gettime(CLOCK_REALTIME, &res2);

    float t = (res2.tv_sec - res1.tv_sec) + (res2.tv_nsec - res1.tv_nsec) * 1.0/1000000000;

    YPH_PRINTF("Total time of executing simple_loop: %fs\n", t);

    return 0;
}

