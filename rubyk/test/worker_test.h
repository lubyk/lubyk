#include "test_helper.h"

class WorkerTest : public TestHelper
{
public:
  void test_run_kill( void ) {
    Root root;
    Worker worker(&root);
    worker.start();
    microsleep(5);
    worker.kill();
  }
  
  void test_running( void ) {
    Root   root;
    Worker worker(&root);
    time_t start = worker.current_time_;
    
    worker.start(); // running in new thread
    microsleep(10);
    worker.kill();
    assert_true(start + 8  <= worker.current_time_);
    assert_true(start + 12 >= worker.current_time_);
  }
};
