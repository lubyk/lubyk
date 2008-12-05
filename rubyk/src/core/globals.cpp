#ifndef _GLOBALS_CPP_
#define _GLOBALS_CPP_

//FIX #include "class.h"
#include "object.h"
#include "values.h"
#include "class.h"

////// GLWINDOW HACK /////
bool gRunning;

typedef void (*plot_thread)(void * pEvent);

plot_thread gGLWindowStartThread;
void * gGLWindowNode;
bool   gQuitGl;

// Node
//FIX pthread_key_t Node::sThisKey     = NULL;
//FIX pthread_key_t Node::sGLThreadKey = NULL;

// Object
unsigned int Object::sIdCounter    = 0;

// Values
Value gNilValue;        // globals declared in values.h
Bang  gBangValue(true); // globals declared in values.h

// StringValue
const  std::string String::sNoString("");

#endif // _GLOBALS_CPP_