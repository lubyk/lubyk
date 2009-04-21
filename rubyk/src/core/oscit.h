#ifndef _RUBYK_OSCIT_H_
#define _RUBYK_OSCIT_H_

#include "oscit/oscit.h"
using namespace oscit;

#define BangIO NilIO
#define gBang gNilValue
#define is_bang is_nil

// TODO move this in a types.h file ?
#define CLASS_URL   "/class"
#define LIB_URL     "/class/lib"
#define INSPECT_URL "/.inspect"
#define RUBYK_URL   "/rubyk"
#define LINK_URL    "/rubyk/link"
#define QUIT_URL    "/rubyk/quit"

#endif