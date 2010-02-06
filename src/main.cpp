#include "rubyk.h"
#include "text_command.h"
#include <iostream>




////// GLWINDOW HACK /////
// instanciated in globals.o
/// extern bool gRunning;
///
/// typedef void (*plot_thread)(void * pEvent);
///
/// extern plot_thread gGLWindowStartThread;
/// extern void * gGLWindowNode;
/// extern bool   gQuitGl;

//OPENGL_HACK Worker * gWorker;
//OPENGL_HACK pthread_t gWorkerThread;
//OPENGL_HACK
//OPENGL_HACK static void * start_thread(void * data)
//OPENGL_HACK {
//OPENGL_HACK   InteractiveCommand cmd(std::cin, std::cout);
//OPENGL_HACK   gWorker->listen_to_command(cmd);
//OPENGL_HACK
//OPENGL_HACK   gWorker->run();
//OPENGL_HACK
//OPENGL_HACK   delete gWorker;
//OPENGL_HACK   gRunning = false;
//OPENGL_HACK   return NULL;
//OPENGL_HACK }
/////////////////////

#define VIEW_PATH "."

int main(int argc, char * argv[])
{
  Planet venus(argc, argv);

  // create '/views' url
  Value error;
  if (!venus.expose_views(VIEW_PATH, &error)) {
    std::cout << error << "\n";
    return -1;
  }

  venus.adopt_command(new CommandLine(std::cin, std::cout));
  venus.adopt_command(new OscCommand("oscit", "_oscit._udp"));
  venus.start_worker();
  venus.wait_for_gui();

  //OPENGL_HACK gGLWindowStartThread = NULL;
  //OPENGL_HACK gGLWindowNode = NULL; /////// GLWINDOW HACK
  //OPENGL_HACK gQuitGl  = false;
  //OPENGL_HACK gRunning = true;
  //OPENGL_HACK
  //OPENGL_HACK struct timespec sleeper;
  //OPENGL_HACK sleeper.tv_sec  = 0;
  //OPENGL_HACK sleeper.tv_nsec = 100 * 1000000; // 100 ms
  //OPENGL_HACK
  //OPENGL_HACK //FIX if (!Node::sGLThreadKey) pthread_key_create(&Node::sGLThreadKey, NULL); // create a key to find 'this' object in new thread
  //OPENGL_HACK //FIX pthread_setspecific(Node::sGLThreadKey,NULL);
  //OPENGL_HACK
  //OPENGL_HACK
  //OPENGL_HACK ////// GLWINDOW HACK /////
  //OPENGL_HACK // this is a hack to put GLWindow inside thread 0
  //OPENGL_HACK pthread_create( &gWorkerThread, NULL, start_thread, NULL);
  //OPENGL_HACK while (gRunning) {
  //OPENGL_HACK   if (gGLWindowStartThread) {
  //OPENGL_HACK     (*gGLWindowStartThread)(gGLWindowNode);
  //OPENGL_HACK     // never reaches here...
  //OPENGL_HACK     break;
  //OPENGL_HACK   }
  //OPENGL_HACK   nanosleep (&sleeper, NULL);
  //OPENGL_HACK }
  //OPENGL_HACK pthread_join( gWorkerThread, NULL);
  //OPENGL_HACK /////////////////////
  //OPENGL_HACK // while (gWorker->run());
  //OPENGL_HACK // delete gWorker;

  return 0;
}