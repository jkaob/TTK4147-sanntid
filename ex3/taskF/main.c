#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t forks[5];
pthread_mutex_t waiter;

void* eat(void* args) {
    int phil = (int)args;

    pthread_mutex_lock(&forks[phil]);
    if (phil == 4)
        pthread_mutex_lock(&forks[0]);
    else
        pthread_mutex_lock(&forks[phil+1]);

    printf("%d eating\n", phil);
    sleep(2);

    pthread_mutex_unlock(&forks[phil]);
    if (phil == 4)
        pthread_mutex_unlock(&forks[0]);
    else
        pthread_mutex_unlock(&forks[phil+1]);
    return NULL;
}

void* eat_safe(void* args){
    unsigned int just_ate = 0;
    for (int i = 0; i<50; i++){

        int phil = (int)args;
        int index = phil;
        if (!just_ate && !pthread_mutex_trylock(&forks[phil])){
            if (phil == 4)
                index = 0;
            else
                index = phil +1;

            if(pthread_mutex_trylock(&forks[index])){
                pthread_mutex_unlock(&forks[phil]);
                printf("%d didn't eat:(\n", phil);
                just_ate = 0;
                sleep(2);
                continue;
            }
        } else{
            printf("%d didn't eat:(\n", phil);
            just_ate = 0;
            sleep(2);
            continue;
        }

        printf("%d eating\n", phil);
        sleep(2);
        if (just_ate)
            continue;

        just_ate = 1;
        pthread_mutex_unlock(&forks[phil]);
        if (phil == 4)
            pthread_mutex_unlock(&forks[0]);
        else
            pthread_mutex_unlock(&forks[phil+1]);
            continue;
        }
    return NULL;
}

void* eat_waiter(void* args){
    int phil = (int)args;

    for (int i=0; i < 50; i++){
        pthread_mutex_lock(&waiter);
        pthread_mutex_lock(&forks[phil]);
        if (phil == 4)
            pthread_mutex_lock(&forks[0]);
        else
            pthread_mutex_lock(&forks[phil+1]);

        pthread_mutex_unlock(&waiter);
        printf("%d eating\n", phil);
        sleep(2);

        pthread_mutex_unlock(&forks[phil]);
        if (phil == 4)
            pthread_mutex_unlock(&forks[0]);
        else
            pthread_mutex_unlock(&forks[phil+1]);


    }
    return NULL;
}

int main()
{
    // 2nd arg is a pthread_mutexattr_t
    for (long i = 0; i < 5; i++){
        pthread_mutex_init(&forks[i], NULL);

    }
    pthread_mutex_init(&waiter, NULL);
    //pthread_mutex_lock(&mtx);

    // Critical section
    //pthread_mutex_unlock(&mtx);
    //pthread_mutex_destroy(&mtx);


    pthread_t threadHandles[5];
    long numThreads = sizeof(threadHandles)/sizeof(*threadHandles);
    
    for(long i = 0; i < numThreads; i++){
        pthread_create(&threadHandles[i], NULL, eat_safe, (void*)i);
    }

    for(long i = 0; i < numThreads; i++){
        pthread_join(threadHandles[i], NULL);
    }   

    for (long i = 0; i < 5; i++){
        pthread_mutex_destroy(&forks[i]);

    } 
    return 0;
}