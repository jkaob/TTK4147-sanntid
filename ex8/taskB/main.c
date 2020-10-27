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

static RT_SEM sem_lock;
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
	rt_task_sleep(US_TO_S*2000);
	RT_SEM_INFO sem_info;
	rt_sem_inquire(&sem_lock, &sem_info);
	rt_printf("Task high trying to lock sem, val: %lu\n", sem_info.count);
	rt_sem_p(&sem_lock, TM_INFINITE);
	rt_sem_inquire(&sem_lock, &sem_info);	
	rt_printf("Task high locked sem, start busy wait,sem val: %lu \n", sem_info.count);
	busy_wait_us(US_TO_S*2);
	rt_printf("Task high finished busy wait\n");
	rt_sem_v(&sem_lock);
	rt_printf("Task high unlocked sem, done\n");
	rt_task_delete(NULL);
}
void task_med(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	
	// Sleep 1 time unit, busy wait 5 time units
	struct rt_task_info temp;
	rt_task_inquire(NULL, &temp);	
	rt_printf("Task mid started, base pri %d, current pri %d \n", temp.bprio, temp.cprio);
	rt_task_sleep(US_TO_S*1000);
	rt_task_inquire(NULL, &temp);	
	rt_printf("Task mid slept, starting busy wait, base pri %d, current pri %d \n", temp.bprio, temp.cprio);
	busy_wait_us(US_TO_S*1);
	rt_printf("Task mid bw 1\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task mid bw 2\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task mid bw 3\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task mid bw 4\n");
	busy_wait_us(US_TO_S*1);
	rt_task_inquire(NULL, &temp);	
	rt_printf("Task med done busy waiting, base pri %d, current pri %d \n", temp.bprio, temp.cprio);
	//rt_printf("Task med done\n");
	rt_task_delete(NULL);
}


void task_low(void* args)
{
	rt_sem_p(&sem_init, TM_INFINITE);
	
	// Lock. busy wait 3 time units. unlock.
	struct rt_task_info temp;
	rt_task_inquire(NULL, &temp);	
	//rt_printf("Task low started, base pri %d, current pri %d \n", temp.bprio, temp.cprio);
	RT_SEM_INFO sem_info;
	rt_sem_inquire(&sem_lock, &sem_info);
	rt_printf("Task low trying to lock sem, val: %lu \n", sem_info.count);
	rt_sem_p(&sem_lock, TM_INFINITE);
	rt_sem_inquire(&sem_lock, &sem_info);
	rt_printf("Task low locked sem, start busy wait,sem val: %lu \n", sem_info.count);
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 1\n");
	busy_wait_us(US_TO_S*1);
	rt_printf("Task low bw 2\n");
	busy_wait_us(US_TO_S*1);
	rt_task_inquire(NULL, &temp);	
	rt_printf("Task low finished busy wait, about to unlock \n");
	rt_sem_v(&sem_lock);
	rt_printf("Task low unlocked sem, done\n");
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
	RT_TASK taskB;
	RT_TASK taskC;
	//RT_TASK task_sync;
	
	rt_sem_create(&sem_lock, "sem_lock", 1, S_FIFO);
	rt_sem_create(&sem_init, "sem_init", 0, S_PRIO);
	
	RT_SEM_INFO sem_info;
	rt_sem_inquire(&sem_lock, &sem_info);
	
	rt_printf("sem value: %lu\n", sem_info.count);
	
	rt_task_create(&taskA, "task_high", 0, 69, T_CPU(0)|T_JOINABLE);
	rt_task_create(&taskB, "task_med", 0, 49, T_CPU(0)|T_JOINABLE);
	rt_task_create(&taskC, "task_low", 0, 19, T_CPU(0)|T_JOINABLE);	
	
	rt_task_shadow(NULL, "main", 99, T_CPU(0));
	
	rt_task_start(&taskA, task_high, NULL);
	rt_task_start(&taskB, task_med,  NULL);
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
	rt_task_sleep(100000000);
	
	rt_printf("Program finished\n");
	
	rt_sem_delete(&sem_init);	
	rt_sem_delete(&sem_lock);
	rt_printf("Semaphores deleted\n");
	
	exit(EXIT_SUCCESS);
	return 0;	
}

