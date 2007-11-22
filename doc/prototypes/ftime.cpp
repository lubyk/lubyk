#include <sys/timeb.h>
#include <cstdio>
#include <time.h>   // ftime
#include <unistd.h> // sleep

static struct timeb gTimeRef;

void init_time_ref()
{
  ftime(&gTimeRef);
}

unsigned long fine_clock()
{
  struct timeb t;
  ftime(&t);
  return ((t.time - gTimeRef.time) * 1000) + t.millitm - gTimeRef.millitm;
}

int main()
{
  unsigned long start, current;
  init_time_ref(); // initialize time ref
  
  start   = fine_clock();
  current = start;
  while(current < start + 1000) {
    current = fine_clock();
    printf("%u\n", current );
  }
  
  
  return 0;
}
