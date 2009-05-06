#include "test_helper.h"
#include "oscit/mutex.h"
#include <sstream>

Mutex gMutexTest_mutex;
std::ostringstream gMutexTest_log(std::ostringstream::out);

void * start_mutex_test_thread(void * data) {  
  gMutexTest_log << "[1:new]";
  gMutexTest_mutex.lock();
  millisleep(2);
  gMutexTest_log << "[1:lock]";
  millisleep(100);
  gMutexTest_mutex.unlock();
  gMutexTest_log << "[1:unlock][1:end]";
  return NULL;
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
    gMutexTest_mutex.lock();
    gMutexTest_log << "[0:lock]";
    pthread_t id;
    // create new thread (will try to get hold of the lock)
    pthread_create( &id, NULL, &start_mutex_test_thread, NULL);
    millisleep(20);
    // release lock() --> other gets hold of it
    gMutexTest_mutex.unlock();
    gMutexTest_log << "[0:unlock]";
    gMutexTest_mutex.lock();
    millisleep(2);
    gMutexTest_log << "[0:lock]";
    gMutexTest_mutex.unlock();
    pthread_join( id, NULL);
    gMutexTest_log << "[joined]";
    
    assert_equal("[0:lock][1:new][0:unlock][1:lock][1:unlock][1:end][0:lock][joined]", gMutexTest_log.str());
  }
};
