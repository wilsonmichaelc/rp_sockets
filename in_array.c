#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

bool in_array(char *val, const char *arr[]){
    int i;
    int size = sizeof(arr) / sizeof(arr[0]);
    for (i=0; i < size; i++) {
        if( memcmp(val, arr[i], (sizeof(arr[i]) - 1) ) == 0 ){
            return true;
        }            
    }
    return false;
}