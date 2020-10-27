#include <native/sem.h>
#include <stdlib.h>
#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <native/mutex.h>

#define CPU_ID 0
RTIME US_TO_S = 1000;

//static RT_SEM sem_lock;
static RT_MUTEX mut_a;
static RT_MUTEX mut_b;
static RT_SEM sem_init;

void busy_wait_us(unsigned long delay)
{
	for (; delay > 0; delay--){
		rt_timer_spin(1000);
	}	
}

void task_high(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	rt_printf("Task high started, going to sleep. \n");
	rt_task_sleep(US_TO_S*1000);
	rt_printf("Task high trying to lock mut B\n");
	rt_mutex_acquire(&mut_b, TM_INFINITE);
	busy_wait_us(US_TO_S*1);
	RT_MUTEX_INFO mut_info_a;
	rt_mutex_inquire(&mut_a, &mut_info_a);
	rt_printf("Task high trying to lock mut A, val: %lu\n", mut_info_a.locked);
	rt_mutex_acquire(&mut_a, TM_INFINITE);
	rt_mutex_inquire(&mut_a, &mut_info_a);	
	rt_printf("Task high locked mut A, start busy wait,sem val: %lu \n", mut_info_a.locked);
	busy_wait_us(US_TO_S*2);
	rt_printf("Task high releasing mut A\n");
	rt_mutex_release(&mut_a);
	rt_printf("Task high releasing mut B\n");
	rt_mutex_release(&mut_b);
	busy_wait_us(US_TO_S*1);
	rt_task_delete(NULL);
}


void task_low(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	
	RT_MUTEX_INFO mut_info_a;
	rt_mutex_inquire(&mut_a, &mut_info_a);
	rt_printf("Task low trying to lock mut A, val: %lu \n", mut_info_a.locked);
	rt_mutex_acquire(&mut_a, TM_INFINITE);
	rt_mutex_inquire(&mut_a, &mut_info_a);
	rt_printf("Task low locked mut A, start busy wait, val: %lu \n", mut_info_a.locked);
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 1\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 2\n");
	busy_wait_us(US_TO_S*1);

	rt_mutex_acquire(&mut_b, TM_INFINITE);
	rt_printf("Task low locked mut B, start busy wait\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 1\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 2\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low releasing mut B\n");
	rt_mutex_release(&mut_b);
	rt_printf("Task low releasing mut A\n");
	rt_mutex_release(&mut_a);
	busy_wait_us(US_TO_S*1);
	rt_task_delete(NULL);
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
	RT_TASK taskC;
	//RT_TASK task_sync;
	
	rt_mutex_create(&mut_a, "mutexA");
	rt_mutex_create(&mut_b, "mutexB");
	rt_sem_create(&sem_init, "sem_init", 0, S_PRIO);
	
	RT_MUTEX_INFO mut_info_a;
	rt_mutex_inquire(&mut_a, &mut_info_a);
	
	rt_printf("mutex value: %lu\n", mut_info_a.locked);
	
	rt_task_create(&taskA, "task_high", 0, 69, T_CPU(0)|T_JOINABLE);
	rt_task_create(&taskC, "task_low", 0, 19, T_CPU(0)|T_JOINABLE);	
	
	rt_task_shadow(NULL, "main", 99, T_CPU(0));
	
	rt_task_start(&taskA, task_high, NULL);
	rt_task_start(&taskC, task_low,  NULL);

	rt_task_sleep(1000000000);
	
	rt_printf("Broadcasting semaphore\n");
	rt_sem_broadcast(&sem_init);
	
//	rt_task_create(&task_sync, NULL, 0, 99, T_CPU(0));
//	rt_task_start(&task_sync, sync_fnc, NULL);

	
	//rt_task_join(&task_sync);
	//rt_task_join(&taskA);
	//rt_task_join(&taskB);
	//rt_task_join(&taskC);
	rt_task_sleep(10000000000);
	
	rt_printf("Program finished\n");
	
	rt_mutex_delete(&mut_a);	
	rt_mutex_delete(&mut_b);	
	rt_sem_delete(&sem_init);
	rt_printf("Semaphores deleted\n");
	
	exit(EXIT_SUCCESS);
	return 0;	
}

