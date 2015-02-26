#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <rp.h>

int in_array(char *val, const char *arr[]){
	int i;
	for(i=0; i<2; i++){
		if( strcmp( arr[i], val ) ){
			return i;
		}
	}
	return -1;
}