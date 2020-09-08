#include "clock_gettime.h"
#include <x86intrin.h>
#include <sched.h>
#define CLOCK_FREQ 2660000000.0


void gettime_method_min_diff()
{
    struct timespec t1;
    struct timespec t2;

    int ns_max = 1000;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t1);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &t2);
        int ns = (int)(t2.tv_nsec - t1.tv_nsec); 
        if (ns >= 0 && ns < ns_max)
        {
            histogram[ns]++;
        }
    }
    for (int i = 0; i < ns_max; i++)
    {
        printf("%d\n", histogram[i]);
    }
}


int main()
{

    gettime_method_min_diff();

    return 0;
}