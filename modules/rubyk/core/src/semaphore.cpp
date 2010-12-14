#include "rubyk/semaphore.h"

namespace rubyk {

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