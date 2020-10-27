#include <native/sem.h>
#include <native/mutex.h>
#include <stdlib.h>
#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <stdio.h>

#define CPU_ID 0


RT_MUTEX mut;
RT_SEM sem_init;

void busy_wait_us(unsigned long delay)
{
	for (; delay > 0; delay--){
		rt_timer_spin(1000);
	}	
}

void task_high(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);

	rt_printf("Task %ld start priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_task_sleep(2000);
	rt_printf("Task %ld executing priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_mutex_acquire(&mut, TM_INFINITE);
	busy_wait_us(2);
	rt_printf("Task %ld executing priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_mutex_release(&mut);
	rt_printf("Task %ld done\n", (long)args);
}

void task_med(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	
	// Sleep 1 time unit, busy wait 5 time units
	rt_printf("Task %ld start priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_task_sleep(1000);
	rt_printf("Task %ld executing priority %d \n", (long)args, rt_task_inquire(NULL));
	busy_wait_us(5);
	rt_printf("Task %ld done\n", (long)args);
}

void task_low(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	
	// Lock. busy wait 3 time units. unlock.
	rt_printf("Task %ld start priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_mutex_acquire(&mut, TM_INFINITE);
	busy_wait_us(3);
	rt_printf("Task %ld executing priority %d \n", (long)args, rt_task_inquire(NULL));
	rt_mutex_release(&mut);
	rt_printf("Task %ld done\n", (long)args);
}




void sync_fnc(void* args)
{
	rt_printf("Broadcasting semaphore\n");
	rt_sem_broadcast(&sem_init);
}

int main(void)
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	
		
	RT_TASK taskA;
	RT_TASK taskB;
	RT_TASK taskC;
	RT_TASK task_sync;
	
	rt_mutex_create(&mut, NULL);
	rt_sem_create(&sem_init, NULL, 0, S_PRIO);
	
	rt_task_create(&taskA, NULL, 0, 69, T_CPU(0));
	rt_task_create(&taskB, NULL, 0, 49, T_CPU(0));
	rt_task_create(&taskC, NULL, 0, 19, T_CPU(0));	

	rt_task_start(&taskA, task_high, (void*)1);
	rt_task_start(&taskB, task_med,  (void*)2);
	rt_task_start(&taskC, task_low,  (void*)3);

	rt_timer_spin(100000000);

	rt_task_create(&task_sync, NULL, 0, 99, T_CPU(0));
	rt_task_start(&task_sync, sync_fnc, NULL);

	rt_timer_spin(100000000);
	
	rt_task_join(&taskA);
	rt_task_join(&taskB);
	rt_task_join(&taskC);
	rt_task_join(&task_sync);
	
	rt_sem_delete(&sem_init);
	rt_mutex_delete(&mut);

	return 0;	
}

