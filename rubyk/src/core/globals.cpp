#ifndef _GLOBALS_CPP_
#define _GLOBALS_CPP_

#include "class.h"

////// GLWINDOW HACK /////
bool gRunning;

typedef void (*plot_thread)(void * pEvent);

plot_thread gGLWindowStartThread;
void * gGLWindowNode;
bool   gQuitGl;

// Node
pthread_key_t Node::sThisKey     = NULL;
pthread_key_t Node::sGLThreadKey = NULL;

// Object
Hash<std::string, Object*> Object::sObjects(OBJECT_HASH_SIZE);
unsigned int Object::sIdCounter    = 0;

#endif // _GLOBALS_CPP_