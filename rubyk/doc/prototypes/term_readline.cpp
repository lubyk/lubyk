// g++ -lreadline term_readline.cpp -o test

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <pthread.h>
#include <fstream>
#include <sys/timeb.h> // ftime

static void microsleep(float microseconds) {
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = (unsigned int)(microseconds * 1000000.0);
  nanosleep (&sleeper, NULL);
}

#define TEST_SIGNAL SIGTERM

pthread_t gReadingThread;
bool      gRunning;
void terminate(int sig) {
  printf("[Terminate]");
  fflush(stdout);
  gRunning = false;
}

void *start_thread1(void *data) {
  printf("[thread start]");
  fflush(stdout);
  char * line;
  signal(TEST_SIGNAL, terminate);
  
  while (gRunning && (line=readline(">")) ) {
    printf("Read \"%s\"", line);
    free(line);
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
  
  char *text = readline("Please select test ('1' for readline, '2' for loop) > ");
  if (text) {
    int test = atoi(text);
    if (test == 1) {
      printf("Testing readline.\n");
      // start_thread won't quit...
      pthread_create( &gReadingThread, NULL, &start_thread1, NULL);
    } else {
      printf("Testing loop.\n");
      // start_thread2 works fine
      pthread_create( &gReadingThread, NULL, &start_thread2, NULL);
    }
    free(text);
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