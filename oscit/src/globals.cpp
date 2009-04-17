#ifndef _OSCIT_GLOBALS_CPP_
#define _OSCIT_GLOBALS_CPP_

#include "oscit/thread.h"

size_t oscit::Object::sIdCounter(0);
oscit::Value oscit::gNilValue;
oscit::Hash  oscit::gEmptyHash(1);
pthread_key_t oscit::Thread::sThisKey;

#endif // _OSCIT_GLOBALS_CPP_