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

////// PLOT HACK /////
bool gRunning;

typedef void (*plot_thread)(void * pEvent);

plot_thread gPlotThread;
void * gPlotThreadData;

pthread_t gRubykThread;

static void * start_thread(void * data)
{
  while (gServer->run());
  delete gServer;
  gRunning = false;
  return NULL;
}
/////////////////////

int main(int argc, char * argv[])
{
  gServer = new Rubyk;
  gPlotThread = NULL;
  gRunning = true; /////// PLOT HACK
  
  if (argc > 1) {
    std::ifstream in(argv[1], std::ios::in);
    std::ostringstream oss;
    oss << in.rdbuf();
    in.close();
    
    Command fCmd(std::cin, std::cout);
    fCmd.set_server(*gServer);
    fCmd.set_silent(true);
    gServer->unlock(); // so the commands are directly processed
      oss << "\n";
      fCmd.parse(oss.str());
    gServer->lock();
    
    fCmd.close();
  }
  
  InteractiveCommand mCmd(std::cin, std::cout);
  gServer->listen_to_command(mCmd);
  signal(SIGTERM, term); // register a SIGTERM handler
  signal(SIGINT,  term);
  
  ////// PLOT HACK /////
  // this is a hack to put Plot opengl inside thread 0
  if (!Node::sThisKey) pthread_key_create(&Node::sThisKey, NULL); // create a key to find 'this' object in new thread
  pthread_create( &gRubykThread, NULL, start_thread, NULL);
  while (gRunning) {
    if (gPlotThread) {
      (*gPlotThread)(gPlotThreadData);
      // never reaches here...
      break;
    }
    sleep(1);
  }
  pthread_join( gRubykThread, NULL);
  /////////////////////
  // while (gServer->run());  
  // delete gServer;
  
  return 0;
}