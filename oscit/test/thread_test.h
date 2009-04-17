#include "test_helper.h"
#include "oscit/thread.h"
#include <sstream>

struct DummyWorker
{
  DummyWorker() : value_(0) {}
  
  void count(Thread *runner) {
    while (runner->should_run()) {
      runner->lock();
      ++value_;
      runner->unlock();
      
      microsleep(10);
    }
  }
  
  void count_high(Thread *runner) {
    runner->high_priority();
    while (runner->should_run()) {
      runner->lock();
      ++value_;
      runner->unlock();
      
      microsleep(10); // should be interrupted
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
    while (should_run()) {
      lock();
      ++value_;
      unlock();
      
      microsleep(10);
    }
  }
  
  int  value_;
};

class ThreadTest : public TestHelper
{  
public:
  void test_create_delete( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill [end]
    microsleep(15);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }
  
  void test_create_kill_sub_class( void ) {
    DummySubWorker * counter = new DummySubWorker;
    counter->start<DummySubWorker, &DummySubWorker::count>(counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill [end]
    microsleep(15);
    counter->kill();
    // should join here
    assert_equal(2, counter->value_);
    delete counter;
    // should not block
  }
  
  void test_create_delete_sub_class( void ) {
    DummySubWorker * counter = new DummySubWorker;
    counter->start<DummySubWorker, &DummySubWorker::count>(counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill [end]
    microsleep(15);
    delete counter;
    // should join here
    // should not block
  }
  
  void test_create_stop( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    microsleep(15);
    runner->stop();
    runner->stop(); // should not lock
    
    // should join here
    assert_equal(2, counter.value_);
    
    delete runner;
  }
  
  // This version sends a SIGINT to stop the thread
  void test_create_kill( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 1 times: +1 ... 5ms kill [end]
    microsleep(5);
    runner->kill();
    runner->kill(); // should not lock
    // should join here
    assert_equal(1, counter.value_);
    
    delete runner;
  }
  
  void test_create_restart( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    microsleep(15);
    runner->kill();
    
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    microsleep(15);
    runner->kill();
    
    // should join here
    assert_equal(4, counter.value_);
    
    delete runner;
  }
  
  void test_create_high_priority( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count_high>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. kill .. 5ms [end]
    microsleep(15);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }
  
  void test_join( void ) {
    assert_true( false ); // TODO
  }
};