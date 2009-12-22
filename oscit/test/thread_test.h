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
#include "oscit/thread.h"
#include <sstream>

std::ostringstream gThreadTest_log(std::ostringstream::out);

struct DummyWorker
{
  DummyWorker() : value_(0) {}

  void count(Thread *runner) {
    runner->thread_ready();
    while (runner->should_run()) {
      runner->lock();
        ++value_;
      runner->unlock();
      Thread::millisleep(20);
    }
  }

  void count_twice(Thread *runner) {
    runner->thread_ready();
    while (runner->should_run() && value_ < 2) {
      runner->lock();
        ++value_;
      runner->unlock();
      Thread::millisleep(20);
    }
  }

  void count_high(Thread *runner) {
    runner->high_priority();
    runner->thread_ready();
    while (runner->should_run()) {
      runner->lock();
        ++value_;
      runner->unlock();
      Thread::millisleep(20); // should be interrupted
    }
  }

  int  value_;
};

struct DummySubWorker : public Thread
{
  DummySubWorker() : value_(0) {}

  ~DummySubWorker() {
    kill();
  }

  void count(Thread *runner) {
    runner->thread_ready();
    while (should_run()) {
      lock();
        ++value_;
      unlock();
      millisleep(20);
    }
  }

  int  value_;
};

class ThreadTest : public TestHelper
{
public:
  void test_create_delete( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 20ms ... +1 ... 10ms .. kill [end]
    millisleep(30);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }

  void test_create_kill_sub_class( void ) {
    DummySubWorker * counter = new DummySubWorker;
    counter->start_thread<DummySubWorker, &DummySubWorker::count>(counter, NULL);
    // let it run 2 times: +1 ... 20ms ... +1 ... 10ms .. kill [end]
    millisleep(30);
    counter->kill();
    // should join here
    assert_equal(2, counter->value_);
    delete counter;
    // should not block
  }

  void test_create_delete_sub_class( void ) {
    DummySubWorker * counter = new DummySubWorker;
    counter->start_thread<DummySubWorker, &DummySubWorker::count>(counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill [end]
    millisleep(30);
    delete counter;
    // should join here
    // should not block
  }

  void test_create_stop( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count>(&counter, NULL);
    millisleep(30);
    runner->kill();
    runner->kill(); // should not lock

    // should join here
    assert_equal(2, counter.value_);

    delete runner;
  }

  // This version sends a SIGINT to stop the thread
  void test_create_kill( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 1 times: +1 ... 10ms kill [end]
    millisleep(10);
    runner->kill();
    runner->kill(); // should not lock
    // should join here
    assert_equal(1, counter.value_);

    delete runner;
  }

  void test_create_restart( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    millisleep(30);
    runner->kill();

    runner->start_thread<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    millisleep(30);
    runner->kill();

    // should join here
    assert_equal(4, counter.value_);

    delete runner;
  }

  void test_create_high_priority( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count_high>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    millisleep(30);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }

  // test fails until we have a good solution: see doc/prototypes/term_readline.cpp
  // void test_kill_readline( void ) {
  //   DummyWorker counter;
  //   Thread *runner = new Thread;
  //   runner->start_thread<DummyWorker, &DummyWorker::read>(&counter, NULL);
  //   // reads input...
  //   millisleep(10);
  //   runner->kill();
  //   // should interrupt
  //   assert_equal(0, counter.value_);
  //   delete runner;
  // }

  void test_join_without_kill( void ) {
    DummyWorker counter;
    Thread *runner = new Thread;
    runner->start_thread<DummyWorker, &DummyWorker::count_twice>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 10ms [done]
    runner->join();
    // should join here
    assert_equal(2, counter.value_);

    delete runner;
  }

  void test_multiple_semaphores( void ) {
    gThreadTest_log.str("");
    Thread m1, m2, m3;
    m2.post();  // increment semaphore ==> 1
    m3.post();  // increment semaphore ==> 1

    // m1 should not be 3. It would if all semaphores were global (same name 'oscit::Thread').
    m1.start_thread<ThreadTest::s_multiple_semaphore_thread>(NULL);  // waits for 'post'
    gThreadTest_log << "[0:start done]";
    m1.post();
    m1.join();
    gThreadTest_log << "[0:join]";
    assert_equal("[1:started][0:start done][1:continue][0:join]", gThreadTest_log.str());
  }

  static void s_multiple_semaphore_thread(Thread *runner) {
    millisleep(100);
    gThreadTest_log << "[1:started]";
    runner->post();
    runner->wait();
    gThreadTest_log << "[1:continue]";
  }
};
