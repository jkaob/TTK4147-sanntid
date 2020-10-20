#include <native/sem.h>
#include <stdlib.h>
#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <stdio.h>

#define CPU_ID 0


RT_SEM sem;

void task_fnc(void* args)
{
	rt_sem_p(&sem, TM_INFINITE);
	rt_printf("Task %ld\n", (long)args);
}

void sync_fnc(void* args)
{
	rt_printf("Broadcasting semaphore\n");
	rt_sem_broadcast(&sem);
}
int main(void)
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	
		
	RT_TASK taskA;
	RT_TASK taskB;
	RT_TASK task_sync;
	
	rt_sem_create(&sem, NULL, 0, S_PRIO);
	
	rt_task_create(&taskA, NULL, 0, 89, T_CPU(0));
	rt_task_create(&taskB, NULL, 0, 79, T_CPU(0));
	
	rt_task_start(&taskA, task_fnc, (void*)1);
	rt_task_start(&taskB, task_fnc, (void*)2);

	rt_timer_spin(100000000);

	rt_task_create(&task_sync, NULL, 0, 99, T_CPU(0));
	rt_task_start(&task_sync, sync_fnc, NULL);

	rt_timer_spin(100000000);
	
	rt_task_join(&taskA);
	rt_task_join(&taskB);
	rt_task_join(&task_sync);
	
	rt_sem_delete(&sem);

	return 0;	
}

