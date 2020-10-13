#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include <sched.h>
#include <time.h>
#include "clock_gettime.h"

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


// Note the argument and return types: 
void* taskA(void* args)
{   
    set_cpu(0);
    int chan = *(int*)args;
	while(1){
		if(!io_read(chan)){
			io_write(chan, 0);
            usleep(5);
			io_write(chan, 1);
		}
	}
}

void* disturbance(void* args)
{   
    set_cpu(0);
    // creates a compiler level memory barrier forcing optimizer 
    // to not re-order memory accesses across the barrier.
    while(1){
        asm volatile("" ::: "memory");
        //printf("I shall not tell lies\n");
    }
}

void* periodic_task(void* args)
{
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);

    struct timespec period = {.tv_sec = 0, .tv_nsec = 500*1000*1000};

    while(1){

        printf("Periodic tasking\n");

        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);

    }
}

void* taskD_periodic(void* args)
{   
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);

    struct timespec period = {.tv_sec = 0, .tv_nsec = 1000*1000};

    set_cpu(0);
    int chan = *(int*)args;
	while(1){
		if(!io_read(chan)){
			io_write(chan, 0);
            usleep(5);
			io_write(chan, 1);
		}
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
	}
}
int main()
{   
    //printf("init\n");
    io_init();
    pthread_t threadA;
    pthread_t threadB;
    pthread_t threadC;
    pthread_t dists[10];
    pthread_create(&threadA, NULL, taskD_periodic, &(int){1});
    pthread_create(&threadB, NULL, taskD_periodic, &(int){2});
    pthread_create(&threadC, NULL, taskD_periodic, &(int){3});

    for (int i = 0; i < 10; i++){
        pthread_create(&dists[i], NULL, disturbance, NULL);
    }

    // joins won't happen
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    for (int i = 0; i < 10; i++){
        pthread_join(dists[i], NULL);
    }
}