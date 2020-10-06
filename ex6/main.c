#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include <sched.h>

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


// Note the argument and return types: 
void* taskA(void* args)
{   
    //set_cpu(1);
    int chan = *(int*)args;
	while(1){
		if(!io_read(chan)){
			io_write(chan, 0);
            usleep(5);
			io_write(chan, 1);
		}
	}
}
int main()
{   
    printf("init\n");
    io_init();
    pthread_t threadA;
    pthread_t threadB;
    pthread_t threadC;
    pthread_create(&threadA, NULL, taskA, &(int){1});
    pthread_create(&threadB, NULL, taskA, &(int){2});
    pthread_create(&threadC, NULL, taskA, &(int){3});
    
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
}