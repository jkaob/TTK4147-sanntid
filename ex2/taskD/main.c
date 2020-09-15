#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "array.h"

int main(){
    Array arr = array_new(2);
    for (long i = 0; i < 200000000; i++){
        array_insertBack(&arr, i);
        printf("%ld\n",arr.capacity);
        //printf("%ld\n",sizeof(arr.data));
    }
    array_destroy(arr);
    
    //array_print(arr);
    //printf("%ld\n", arr.capacity);
    //printf("%p\n", arr.data);

    // long size = 1;
    // for (int i = 0;i<20;i++){
    //     size*=2;
    //     printf("%ld\n", size);
    // }

    return 0;
    
}