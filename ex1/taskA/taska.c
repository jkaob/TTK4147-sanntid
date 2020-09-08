#include <stdio.h>

#include "clock_gettime.h"
#define TICKS_PER_SEC 2660000000

void busy_wait_times()
{
    static struct tms start_time;
    static struct tms end_time;
    
    clock_t wait_time = 100; //time in hundreds/ 10 ms.
    while (end_time.tms_stime + end_time.tms_utime - start_time.tms_stime - start_time.tms_utime < wait_time)
    {
        times(&end_time);
        //printf("%ld", end - start);
    }
}

int main(int argc, char *agrv[]){
    busy_wait_times();
    return 0;
}