#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


long global_var = 0;

// Note the argument and return types: 
void* fn(void* args)
{   
    long local_var = 0;
    for (unsigned int i = 0; i <50000000; i++){
        global_var++;
        local_var++;
    }
    printf("global: %ld\n", global_var);
    printf("local: %ld\n", local_var);

    return NULL;
}
int main()
{
    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, fn, NULL);
    fn(NULL);
    pthread_join(threadHandle, NULL);
}