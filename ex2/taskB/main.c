#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "array.h"

int main(){
    Array arr = array_new(5);
    
    for (long i = 0; i < 10; i++){
        array_insertBack(&arr, i);
        printf("%ld \n",array_back(arr));
    }   
    array_print(arr);
    
    array_destroy(arr);
}