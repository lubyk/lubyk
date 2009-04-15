#include "test_helper.h"

class WorkerTest : public TestHelper
{
public:
  void test_run_kill( void ) {
    Root root;
    Worker worker(&root);
    worker.run();
    microsleep(5);
    worker.kill();
  }
  
  void test_running( void ) {
    Root   root;
    Worker worker(&root);
    time_t start = worker.current_time_;
    
    worker.run(); // running in new thread
    microsleep(10);
    worker.kill();
    assert_true(start + 9  <= worker.current_time_);
    assert_true(start + 11 >= worker.current_time_);
  }
};
