#include "clock_gettime.h"
#include <x86intrin.h>
#define CLOCK_FREQ 2660000000.0

double gettime_method_avg()
{
    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    double n = 10*1000*1000;
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    { // read timer}
        clock_gettime(CLOCK_MONOTONIC, &end);
    }
    struct timespec passed = timespec_sub(end, start);
    //
    return (passed.tv_sec*1000000000.0 + passed.tv_nsec)/n; //in nanoseconds
}

double rdtsc_method_avg(){
    int start = __rdtsc();
    int end;
    //double n = 10 * 1000 * 1000;
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    { // read timer}
        end = __rdtsc();
    }

    return (end - start) * 100.0 / CLOCK_FREQ; //in nanoseconds
}

double times_method_avg(){
    static struct tms start_time;
    static struct tms end_time;

    //n = 10*1000*1000
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    { // read timer}
        times(&end_time);
    }

    long passed = end_time.tms_stime + end_time.tms_utime - start_time.tms_stime - start_time.tms_utime; //in 10ms
    //passed*1000/n
    
    return (passed); //in nanoseconds
}

void gettime_method_min_diff()
{
    struct timespec t1;
    struct timespec t2;

    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t1);
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

void rdtsc_method_min_diff()
{
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {
        unsigned long t1 = __rdtsc();
        unsigned long t2 = __rdtsc();
        
        int ns = (int)(t2 - t1)/2.66; // (t2 - t1) * ??
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

void times_method_min_diff()
{
    static struct tms start_time;
    static struct tms end_time;

    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);
    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {
        times(&start_time);
        times(&end_time);

        int ns = (int)(end_time.tms_stime + end_time.tms_utime - start_time.tms_stime - start_time.tms_utime)*10*1000*1000;
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

    // avg time to read time value
    /*
    printf("rdt: %f ns\n", rdtsc_method_avg());
    printf("gettime: %f ns\n", gettime_method_avg());
    printf("times: %f ns\n", times_method_avg());
    */
    //rdtsc_method_min_diff();
    //gettime_method_min_diff();
    times_method_min_diff();

    return 0;
}