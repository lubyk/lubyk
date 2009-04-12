#include "test_helper.h"
#include "oscit/thread.h"
#include <sstream>

struct DummyWorker
{
  DummyWorker() : value_(0), quit_(false) {}
  
  void count(Thread *runner) {
    while (runner->run()) {
      runner->lock();
      ++value_;
      runner->unlock();
      
      microsleep(10);
    }
  }
  
  void count2(Thread *runner) {
    while (!quit_) {
      runner->lock();
      ++value_;
      runner->unlock();
      
      sleep(10000); // should be interrupted
    }
  }
  
  void count_high(Thread *runner) {
    runner->high_priority();
    while (!quit_) {
      runner->lock();
      ++value_;
      runner->unlock();
      
      microsleep(10); // should be interrupted
    }
  }
  
  void terminate(Thread *runner) {
    quit_ = true;
  }
  
  int  value_;
  bool quit_;
};

class ThreadTest : public TestHelper
{  
public:
  void test_create_delete( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit .. 5ms [end]
    microsleep(15);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }
  
  // This version sends a SIGINT to stop the thread
  void test_create_delete_signals( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start_using_signals<DummyWorker, &DummyWorker::count2>(&counter, NULL);
    // let it run 1 times: +1 ... looooong sleep .. SIGTERM [end]
    microsleep(15);
    delete runner;
    // should join here
    assert_equal(1, counter.value_);
  }
  
  void test_create_quit( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit .. 5ms [end]
    microsleep(15);
    runner->quit();
    runner->quit(); // should not lock
    
    // should join here
    assert_equal(2, counter.value_);
    
    delete runner;
  }
  
  // This version sends a SIGINT to stop the thread
  void test_create_quit_signals( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start_using_signals<DummyWorker, &DummyWorker::count2>(&counter, NULL);
    // let it run 1 times: +1 ... looooong sleep .. SIGTERM [end]
    microsleep(15);
    runner->quit();
    runner->quit(); // should not lock
    // should join here
    assert_equal(1, counter.value_);
    
    delete runner;
  }
  
  void test_create_restart( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit .. 5ms [end]
    microsleep(15);
    runner->quit();
    
    runner->start<DummyWorker, &DummyWorker::count>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit .. 5ms [end]
    microsleep(15);
    runner->quit();
    
    // should join here
    assert_equal(4, counter.value_);
    
    delete runner;
  }
  
  // This version sends a SIGINT to stop the thread
  void test_create_restart_signals( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start_using_signals<DummyWorker, &DummyWorker::count2>(&counter, NULL);
    // let it run 1 times: +1 ... looooong sleep .. SIGTERM [end]
    microsleep(15);
    runner->quit();
    
    counter.quit_ = false;
    runner->start_using_signals<DummyWorker, &DummyWorker::count2>(&counter, NULL);
    // let it run 1 times: +1 ... looooong sleep .. SIGTERM [end]
    microsleep(15);
    runner->quit();
    
    // should join here
    assert_equal(2, counter.value_);
    
    delete runner;
  }
  
  void test_create_high_priority( void ) {
    DummyWorker counter;
    Thread * runner = new Thread;
    runner->start_using_signals<DummyWorker, &DummyWorker::count_high>(&counter, NULL);
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit .. 5ms [end]
    microsleep(15);
    delete runner;
    // should join here
    assert_equal(2, counter.value_);
  }
};