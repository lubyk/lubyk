/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#include "lubyk/semaphore.h"

namespace lubyk {

Semaphore::Semaphore(int resource_count) : resource_count_(resource_count), semaphore_(NULL) {
//#ifdef __macosx__
  semaphore_ = sem_open("core::Semaphore", O_CREAT, 0, resource_count_);
  if (semaphore_ == NULL) {
    fprintf(stderr, "Could not open semaphore 'core::Semaphore' (%s)\n", strerror(errno));
  } else {
    // transform this process wide semaphore into an unamed semaphore.
    if (sem_unlink("core::Semaphore") < 0) {
      fprintf(stderr, "Could not unlink semaphore 'core::Semaphore' (%s)\n", strerror(errno));
    }
  }
//#else
//  if (sem_init(&semaphore_, 0, 0) < 0) {
//    fprintf(stderr, "Could not init semaphore (%s)\n", strerror(errno));
//  }
//#endif
}

Semaphore::~Semaphore() {
  if (semaphore_) {
//#ifdef __macosx__
    if (sem_close(semaphore_) < 0) {
      fprintf(stderr, "Could not close semaphore (%s)\n", strerror(errno));
      assert(false);
    }
//#else
//    sem_destroy(*semaphore_);
//#endif
  }
}


}  // core