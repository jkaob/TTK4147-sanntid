#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "array.h"

int main(){
    Array arr = array_new(10);
    
    for (long i = 0; i < 11; i++){
        array_insertBack(&arr, i);
        //printf("%ld \n",array_back(arr));
    }
    array_print(arr);
    printf("%ld\n", arr.capacity);
    array_destroy(arr);
    
}