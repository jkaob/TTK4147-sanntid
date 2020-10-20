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

#define CPU 0

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* disturbance(void* args) {   
	set_cpu(CPU);
	// creates a compiler level memory barrier forcing optimizer 
	// to not re-order memory accesses across the barrier.
	while(1){
	//asm volatile("" ::: "memory");
	//printf("I shall not tell lies\n");
	}
	return NULL;
}

void task_fnc(void* args)
{	
	unsigned long duration = 10000000000; // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
	int chan = *(int*)args;
	
	while(1)
	{
		//do work
		if(!io_read(chan)){
			io_write(chan, 0);
            		rt_timer_spin(5000);
			io_write(chan, 1);
		}
		

		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		 }
		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}
	//return NULL;
}

//void* task_periodic(void* args) {
//	unsigned long duration = 10000000000; // 10 second timeout
//	unsigned long endTime = rt_timer_read() + duration;
//	rt_printf("testing periodic task\n");
//	
//	return NULL;
//}

int main()
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	//io_init();
	
    	//pthread_t dists[10];

   //    for (int i = 0; i < 10; i++){
   //      	pthread_create(&dists[i], NULL, disturbance, NULL);
   //	}
	
	RT_TASK taskA;
	//RT_TASK taskB;
	//RT_TASK taskC;

	rt_task_create(&taskA, "task A", 0, 1, T_CPU(CPU));
	//rt_task_create(&taskB, "task B", 0, 1, T_CPU(CPU));
	//rt_task_create(&taskC, "task C", 0, 1, T_CPU(CPU));
	
	rt_task_start(&taskA, &task_fnc, (int*)1);
	//rt_task_start(&taskB, &task_fnc, (int*)2);
	//rt_task_start(&taskC, &task_fnc, (int*)3);

	//RT_TASK periodic_task[3];
	//for (int i = 0; i < 3; i++) {
	//	rt_task_create(&task_periodic[i], "task", 0, 1, T_CPU(CPU));
	//}
	
	//rt_task_set_periodic(RT_TASK *task, RTIME idate, period);

	
    	// joins won't happen

    	//for (int i = 0; i < 10; i++){
        //	pthread_join(dists[i], NULL);
    	//}
	
	rt_task_delete(&taskA);
	rt_task_join(&taskA);
	//rt_task_join(&taskB);
	//rt_task_join(&taskC);
	while(1);
	return 0;
}
