/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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

// ======================================== Worker
pthread_key_t Worker::sThisKey     = NULL;
//FIX pthread_key_t Node::sGLThreadKey = NULL;

// ======================================== Object
#include "oscit/globals.cpp"

#ifdef _TESTING_
size_t Data::sIdCounter = 0;
bool   Data::sShowId = true;
#endif

// ======================================== Values
Value gNilValue;        // globals declared in values.h
Bang  gBangValue(true); // globals declared in values.h

// ======================================== StringValue
const  std::string String::sNoString("");

#endif // _GLOBALS_CPP_