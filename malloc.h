#include <stdio.h>
#include <stdbool.h>

#define HEAP_SIZE 1048576 //Heap size
#define Max_HEAP_CHUNK 1000 //MAx number of memory chuncks the heap can be devided in to

typedef struct{
  void* start; //address
  size_t size; //size
  int8_t state; //state
}HeapChunk;//An object defining a chunk of heap

char heap[HEAP_SIZE]; //Heap
HeapChunk metadata[Max_HEAP_CHUNK]; //data structure to keep track of heap memory state

bool firstRun = true; //determine firstRun of allocator function


void* hmalloc(size_t size){
  int entry; //Where in the metadata is the allocated chunck entry
  if (size <= 0) //Return null if 0bytes of memory is asked
    return NULL;
   
   //return the heap address for first time memory allocation 
  if (firstRun){
    metadata[0].start = heap;
    metadata[0].size = HEAP_SIZE;
    firstRun = false;
  }
  
  void* result = NULL; //Pointer to be returned
  
  //Loop through every chunck to find a free one
  for (int i=0; i < Max_HEAP_CHUNK; i++){
    if (metadata[i].start == 0) //pass invalid chunk
      continue;
    
    //Allocate a valid chuck of memory  
    if (metadata[i].size >= size && metadata[i].state == 0){
      if (metadata[i].size > size && metadata[i+1].state == 1)
        continue; //pass the chunck if it results in fragmentation
      entry = i;
      result = metadata[i].start;
      metadata[i].state = 1;
      
      //Merge the remaining part of the allocated chunk if the next chunck if free or add it in the chunck list in place of an invalid chunk if there is one
      if (size < metadata[i].size && metadata[i+1].state == 0){
        metadata[i+1].size = metadata[i].size - size ;
        metadata[i+1].start = metadata[i].start + size;
      }
            
    metadata[i].size = size; //Update the size of allocated chunck
    break;
    }
  }
  //printf("Allocated chunck of size: %zu @ addrr: %p metadata entry: %d\n", size, result, entry);
  return result;
}


void hmfree(void* ptr){
  int entry;
  //Loop through chuncks to find the one to be freed
  for (int i=0; i < Max_HEAP_CHUNK; i++){
    if (metadata[i].start == ptr){
      if (metadata[i].state == 0)
        break;
        
      entry = i;
      metadata[i].state = 0; // free the chunck
      //Merge the next chunck with the newly freed one if it's free
      if (metadata[i+1].state == 0){ 
        metadata[i].size += metadata[i+1].size;
        metadata[i+1].start = 0;
     }
     //Merge the previous chunck also if it's free and the freed chunck is not chunk 0 (Beginnig of heap)
      if (i != 0 ){
        if (metadata[i-1].state == 0){
          metadata[i-1].size += metadata[i].size;
          metadata[i].start = 0;
        }
      }
      break;
    }
  }
  //printf("Free chunck of size: %zu @ addrr: %p entry: %d\n", metadata[entry].size, metadata[entry].start, entry);
  return;
}