#include "planet.h"
#include "command.h"
#include <iostream>
#include "globals.cpp"




////// GLWINDOW HACK /////
// instanciated in globals.o
/// extern bool gRunning;
/// 
/// typedef void (*plot_thread)(void * pEvent);
/// 
/// extern plot_thread gGLWindowStartThread;
/// extern void * gGLWindowNode;
/// extern bool   gQuitGl;

//OPENGL_HACK Planet * gPlanet;
//OPENGL_HACK pthread_t gPlanetThread;
//OPENGL_HACK 
//OPENGL_HACK static void * start_thread(void * data)
//OPENGL_HACK { 
//OPENGL_HACK   InteractiveCommand cmd(std::cin, std::cout);
//OPENGL_HACK   gPlanet->listen_to_command(cmd);
//OPENGL_HACK   
//OPENGL_HACK   gPlanet->run();
//OPENGL_HACK   
//OPENGL_HACK   delete gPlanet;
//OPENGL_HACK   gRunning = false;
//OPENGL_HACK   return NULL;
//OPENGL_HACK }
/////////////////////

int main(int argc, char * argv[])
{  
  Planet venus(argc, argv);
  InteractiveCommand cmd(std::cin, std::cout);
  venus.listen_to_command(cmd);
  venus.run();
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
  //OPENGL_HACK pthread_create( &gPlanetThread, NULL, start_thread, NULL);
  //OPENGL_HACK while (gRunning) {
  //OPENGL_HACK   if (gGLWindowStartThread) {
  //OPENGL_HACK     (*gGLWindowStartThread)(gGLWindowNode);
  //OPENGL_HACK     // never reaches here...
  //OPENGL_HACK     break;
  //OPENGL_HACK   }
  //OPENGL_HACK   nanosleep (&sleeper, NULL);
  //OPENGL_HACK }
  //OPENGL_HACK pthread_join( gPlanetThread, NULL);
  //OPENGL_HACK /////////////////////
  //OPENGL_HACK // while (gPlanet->run());  
  //OPENGL_HACK // delete gPlanet;
  
  return 0;
}