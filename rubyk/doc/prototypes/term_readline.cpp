// g++ -lreadline term_readline.cpp -o test
#include <stdio.h>
#include <setjmp.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <pthread.h>
#include <fstream>
#include <sys/timeb.h> // ftime



static jmp_buf buf;

static void microsleep(float microseconds) {
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = (unsigned int)(microseconds * 1000000.0);
  nanosleep (&sleeper, NULL);
}

#define TEST_SIGNAL SIGTERM

pthread_t gReadingThread;
bool      gRunning;

extern int rl_catch_signals;

void terminate(int sig) {
  printf("[Terminate]");
  fflush(stdout);
  gRunning = false;
  
  rl_free_line_state();      // if it was working, longjmp would not be needed ! It seems to do nothing...
  rl_cleanup_after_signal(); // not doing anyting...
  longjmp(buf,1);
}

void *start_thread1(void *data) {
  printf("[thread start]");
  fflush(stdout);
  char * line;
  signal(TEST_SIGNAL, terminate);
  rl_catch_signals = 0;
  if ( !setjmp(buf) ) {
    // normal loop
    
    while (gRunning && (line=readline(">")) ) {
      printf("Read \"%s\"", line);
      free(line);
    }
  } else {
    // jump back after interrupt
  }
  
  return NULL;
}

void *start_thread2(void *data) {
  printf("[thread start]");
  fflush(stdout);
  signal(TEST_SIGNAL, terminate);
  
  while (gRunning) {
    printf(".");
    fflush(stdout);
    microsleep(100);
  }
  return NULL;
}


int main() {
  gRunning = true;
  rl_catch_signals = 0;
  if (true) {
    printf("Testing readline.\n");
    // start_thread won't quit...
    pthread_create( &gReadingThread, NULL, &start_thread1, NULL);
  } else {
    printf("Testing loop.\n");
    pthread_create( &gReadingThread, NULL, &start_thread2, NULL);
  }
  
  printf("[New created]");
  fflush(stdout);
  sleep(1);
  printf("[Kill!]");
  fflush(stdout);
  pthread_kill(gReadingThread, TEST_SIGNAL);
  pthread_join(gReadingThread, NULL);
  printf("[Joined]\n");
}