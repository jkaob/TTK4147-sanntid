#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


long global_var = 0;
sem_t sem;

// Note the argument and return types: 
void* fn(void* args)
{   
    long local_var = 0;
    for (unsigned int i = 0; i <50000000; i++){
        sem_wait(&sem);
        global_var++;
        sem_post(&sem);
        local_var++;
    }
    printf("local: %ld\n", local_var);

    return NULL;
}
int main()
{
    sem_init(&sem, 0, 1);
    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, fn, NULL);
    fn(NULL);
    pthread_join(threadHandle, NULL);
    printf("global: %ld\n", global_var);
    sem_destroy(&sem);
}