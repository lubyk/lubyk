// compile with
// g++ -o test load.cpp

#include <dlfcn.h>
#include <cstdio>
#include "load_test.h"

// for help to create a portable version of this load function, read Ruby's dln.c file.
int load(char * file, char * init_name, Dummy &master)
{
  void *image;
  void (*function)(Dummy&);
  const char *error = 0;
  
  // load shared extension image into memory
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if (error = dlerror()) 
      printf(" %s\n", error);
    else
      printf("\n");
    return 0;
  }
  
  // get 'init' function into the image
  function = (void(*)(Dummy&))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    printf("Symbol '%s' not found in '%s'.",init_name,file);
    if (error = dlerror()) 
      printf(" %s\n", error);
    else
      printf("\n");
    return 0;
  }
  
  // call 'init', passing the registration object
  (*function)(master);

  return 1;
}

int main ()
{
  Dummy master;
  printf("\nMaster before load :\n");
  master.print();
  printf("--------------------\n");
  load("load_test.bundle", "init", master);
  
  printf("Master after load   \n");
  master.print();
  printf("--------------------\n");
  
}