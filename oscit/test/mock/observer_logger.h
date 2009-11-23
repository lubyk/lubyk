#ifndef OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_
#define OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_

#include "oscit/observer.h"
#include "mock/logger.h"

struct MockObservable : public Observable {
  MockObservable(const char *name) : name_(name) {}
  const char *name_;
};

class ObserverLogger : public Observer, protected MockLogger {
public:
  ObserverLogger(const char *name, std::ostringstream *stream) :
                     MockLogger(name, stream), x_(NULL), y_(NULL) {}

  void set_x(MockObservable *x) {
    set_and_hold(&x_, x);
  }

  void set_y(MockObservable *y) {
    set_and_hold(&y_, y);
  }

  /** This method invalidates an instance variable.
   */
  virtual void observer_lock() {
    log("lock", (x_ ? x_->name_ : "NULL"), (y_ ? y_->name_ : "NULL"));
  }

  /** Stop observing an object contained in ivar. If you are in a multi-threaded
   *  environment, you should overwrite this method.
   * @params void *ivar_ptr address of the instance variable containing the value (already set to NULL).
   */
  virtual void observer_unlock() {
     log("unlock", (x_ ? x_->name_ : "NULL"), (y_ ? y_->name_ : "NULL"));
  }

  MockObservable *x_;
  MockObservable *y_;
};


#endif // OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_