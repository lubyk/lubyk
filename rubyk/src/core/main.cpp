#include "rubyk.h"
#include "command.h"
#include <csignal>
#include <iostream>
#include <fstream>

Rubyk * gServer;

void term(int sig)
{
  gServer->quit();
}

////// GLWINDOW HACK /////
// instanciated in globals.o
extern bool gRunning;

typedef void (*plot_thread)(void * pEvent);

extern plot_thread gGLWindowStartThread;
extern void * gGLWindowNode;
extern bool   gQuitGl;

pthread_t gRubykThread;

static void * start_thread(void * data)
{  
  InteractiveCommand mCmd(std::cin, std::cout);
  gServer->listen_to_command(mCmd);
  
  signal(SIGTERM, term); // register a SIGTERM handler
  signal(SIGINT,  term);
  
  while (gServer->run());
  delete gServer;
  gRunning = false;
  return NULL;
}
/////////////////////

int main(int argc, char * argv[])
{
  gServer = new Rubyk;
  gGLWindowStartThread = NULL;
  gGLWindowNode = NULL; /////// GLWINDOW HACK
  gQuitGl  = false;
  gRunning = true;
  
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = 100 * 1000000; // 100 ms
  
  if (argc > 1) {
    std::ifstream in(argv[1], std::ios::in);
    std::ostringstream oss;
    oss << in.rdbuf();
    in.close();
    
    Command  * fCmd;
    fCmd = new Command(std::cin, std::cout); // we use new because cleanup code is not executed for this thread due to opengl glutMainLoop
    fCmd->set_server(*gServer);
    fCmd->set_silent(true);
    gServer->unlock(); // so the commands are directly processed
      oss << "\n";
      fCmd->parse(oss.str());
    gServer->lock();
    
    fCmd->close();
    delete fCmd;
  }
  
  if (!Node::sThisKey)     pthread_key_create(&Node::sThisKey,     NULL); // create a key to find 'this' object in new thread
  if (!Node::sGLThreadKey) pthread_key_create(&Node::sGLThreadKey, NULL); // create a key to find 'this' object in new thread
  
  ////// GLWINDOW HACK /////
  // this is a hack to put GLWindow inside thread 0
  pthread_create( &gRubykThread, NULL, start_thread, NULL);
  while (gRunning) {
    if (gGLWindowStartThread) {
      (*gGLWindowStartThread)(gGLWindowNode);
      // never reaches here...
      break;
    }
    nanosleep (&sleeper, NULL);
  }
  pthread_join( gRubykThread, NULL);
  /////////////////////
  // while (gServer->run());  
  // delete gServer;
  
  return 0;
}