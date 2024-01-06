#include "debug.h"

int nballoc = 0;
int nbfree = 0;

void* mymalloc(size_t size){
  nballoc++;
  return malloc(size);
}

void myfree(void *ptr)
{
  nbfree++;
  free(ptr);
}