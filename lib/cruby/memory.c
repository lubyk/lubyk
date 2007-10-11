#include "rubyk.h"

void * 
rk_alloc (size_t memsize)
{
  void * ptr;
  ptr = malloc(memsize);
  if (ptr == NULL) {
    rk_die("Memory allocation failure\n");
  }
  return ptr;
}

void
rk_free (void * ptr)
{
  free(ptr);
}