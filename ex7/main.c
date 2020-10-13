#include <stdlib.h>
#include <rtdk.h>
#include <native/task.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include <sched.h>
#include <native/types.h>
#include <nucleus/timer.h>
#include <native/timer.h>
#include <asm-generic/xenomai/timeconv.h>

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
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

void* task_fnc(void* args)
{
	unsigned long duration = 10000000000; // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
	
	while(1)
	{
		//do work
		
		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		 }
		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}
}

int main()
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	io_init();
	
    	pthread_t dists[10];

    	for (int i = 0; i < 10; i++){
        	pthread_create(&dists[i], NULL, disturbance, NULL);
    	}
	
	RT_TASK taskA;
	RT_TASK taskB;
	RT_TASK taskC;

	rt_task_create(&taskA, "task A", 0, 1, T_CPU(0));
	rt_task_create(&taskB, "task B", 0, 1, T_CPU(0));
	rt_task_create(&taskC, "task C", 0, 1, T_CPU(0));

	
    	// joins won't happen

    	for (int i = 0; i < 10; i++){
        	pthread_join(dists[i], NULL);
    	}
	return 0;
}
