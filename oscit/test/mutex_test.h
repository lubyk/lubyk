/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "test_helper.h"
#include "oscit/mutex.h"
#include <sstream>

Mutex gMutexTest_mutex;
std::ostringstream gMutexTest_log(std::ostringstream::out);

void start_mutex_test_thread(Thread* runner) {
  gMutexTest_log << "[1:new]";
  runner->thread_ready();
  gMutexTest_mutex.lock();
  gMutexTest_log << "[1:lock]";
  runner->post();
  gMutexTest_mutex.unlock();
  gMutexTest_log << "[1:unlock][1:end]";
}

static void lock_method(Thread* runner) {
  ScopedLock lock(gMutexTest_mutex);
  gMutexTest_log << "[1:lock]";
  runner->post();
}

static void start_mutex_test_thread_scoped_lock(Thread* runner) {
  gMutexTest_log << "[1:new]";
  runner->thread_ready();
  lock_method(runner);
  gMutexTest_log << "[1:unlock][1:end]";
}


class MutexTest : public TestHelper
{
public:
//  we have removed support for recursive locks (see if it works without first)
//  void test_double_lock( void ) {
//    Mutex mutex;
//    mutex.lock();
//    mutex.lock();
//    // the test would hang and would never reach here if it fails
//    assert_true( true ); // passed !
//  }

  void test_bad_unlock( void ) {
    Mutex mutex;
    mutex.unlock();
    // should not raise any error
    assert_true( true ); // passed !
  }

  void test_two_threads( void ) {
    Thread runner;
    gMutexTest_mutex.lock();
    gMutexTest_log << "[0:lock]";
    // create new thread (will try to get hold of the lock)
    runner.start_thread<start_mutex_test_thread>(NULL);
    // release lock() --> other gets hold of it
    gMutexTest_log << "[0:unlock]";
    gMutexTest_mutex.unlock();
    runner.wait();
    gMutexTest_mutex.lock();
    gMutexTest_log << "[0:lock]";
    gMutexTest_mutex.unlock();
    runner.join();
    gMutexTest_log << "[joined]";

    assert_equal("[0:lock][1:new][0:unlock][1:lock][1:unlock][1:end][0:lock][joined]", gMutexTest_log.str());
  }

  void test_two_threads_scoped_lock( void ) {
    gMutexTest_log.str("");
    Thread runner;
    {
      ScopedLock lock(gMutexTest_mutex);
      gMutexTest_log << "[0:lock]";
      // create new thread (will try to get hold of the lock)
      runner.start_thread(start_mutex_test_thread_scoped_lock, NULL);
      // release lock() --> other gets hold of it
      gMutexTest_log << "[0:unlock]";
    }
    runner.wait();
    {
      ScopedLock lock(gMutexTest_mutex);
      gMutexTest_log << "[0:lock]";
    }
    runner.join();
    gMutexTest_log << "[joined]";

    assert_equal("[0:lock][1:new][0:unlock][1:lock][1:unlock][1:end][0:lock][joined]", gMutexTest_log.str());
  }
  //void test_two_threads_scoped_lock( void ) {
  //  Thread runner;
  //  {
  //    ScopedLock lock(gMutexTest_mutex);
  //    gMutexTest_log << "[0:lock]";
  //    // create new thread (will try to get hold of the lock)
  //    runner.start_thread(start_mutex_test_thread_scoped_lock, NULL);
  //    // release lock() --> other gets hold of it
  //    gMutexTest_log << "[0:unlock]"; 
  //  }
  //  runner.wait();
  //  { 
  //    ScopedLock lock(gMutexTest_mutex);
  //    gMutexTest_mutex.lock();
  //    gMutexTest_log << "[0:lock]";
  //  }
  //  runner.join();
  //  gMutexTest_log << "[joined]";
  //
  //  assert_equal("[0:lock][1:new][0:unlock][1:lock][1:unlock][1:end][0:lock][joined]", gMutexTest_log.str());
  //}
};
