#ifndef _GLOBALS_CPP_
#define _GLOBALS_CPP_

//FIX #include "class.h"
#include "oscit.h"
#include "values.h"
#include "class.h"

////// GLWINDOW HACK /////
bool gRunning;

typedef void (*plot_thread)(void * pEvent);

plot_thread gGLWindowStartThread;
void * gGLWindowNode;
bool   gQuitGl;

// Worker
pthread_key_t Worker::sThisKey     = NULL;
//FIX pthread_key_t Node::sGLThreadKey = NULL;

// Object
#include "oscit/globals.cpp"

#ifdef _TESTING_
size_t Data::sIdCounter = 0;
bool   Data::sShowId = true;
#endif

// Values
Value gNilValue;        // globals declared in values.h
Bang  gBangValue(true); // globals declared in values.h

// StringValue
const  std::string String::sNoString("");

#endif // _GLOBALS_CPP_