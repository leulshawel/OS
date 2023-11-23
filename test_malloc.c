#include <stdio.h>
#include "malloc.h"



void dump_metadata(){

	printf("State 0=free 1=allocated\n");
	for (int i=0; i <=Max_HEAP_CHUNK; i++){
		printf("chunck start: %p size: %zu state: %d\n", metadata[i].start, metadata[i].size, metadata[i].state);
	}
}

int main(){

	void* ptrArray[50]; 
	
	//Allocate 50 memory chunck of different [i] sizes
	for (int i=1; i <= 50; i++){
		ptrArray[i] = hmalloc(i);
	}
	
	//Free some of the allocated memory chuncks
	for (int i=1; i <= 50; i++){
		if (i%2 == 0 || i%5 == 0){
			hmfree(ptrArray[i]);
		}
	}

	dump_metadata(); //look at the metadata to check if all allocations and releases heppened successfully


	return 0;
}
