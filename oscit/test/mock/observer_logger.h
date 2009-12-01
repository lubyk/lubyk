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
  enum {
    VarTest,
    ListXTest,
    ListYTest
  };
  ObserverLogger(const char *name, std::ostringstream *stream)
      : MockLogger(name, stream),
        mode_(VarTest),
        x_(NULL),
        y_(NULL) {}

  void set_x(MockObservable *x) {
    set_and_hold(&x_, x);
  }

  void set_y(MockObservable *y) {
    set_and_hold(&y_, y);
  }

  void append_x(MockObservable *x) {
    append_and_hold(&list_x_, x);
  }

  void append_y(MockObservable *y) {
    append_and_hold(&list_y_, y);
  }

  void remove_x(MockObservable *x) {
    remove_from_list(&list_x_, x);
  }

  void remove_y(MockObservable *y) {
    remove_from_list(&list_y_, y);
  }

  /** This method invalidates an instance variable.
   */
  virtual void observer_lock() {
    switch(mode_) {
    case VarTest:
      log("lock", (x_ ? x_->name_ : "NULL"), (y_ ? y_->name_ : "NULL"));
      break;
    case ListXTest:
      log("lock", list_x_.size());
      break;
    case ListYTest:
      log("lock", list_y_.size());
      break;
    }
  }

  /** Stop observing an object contained in ivar. If you are in a multi-threaded
   *  environment, you should overwrite this method.
   * @params void *ivar_ptr address of the instance variable containing the value (already set to NULL).
   */
  virtual void observer_unlock() {
     switch(mode_) {
     case VarTest:
        log("unlock", (x_ ? x_->name_ : "NULL"), (y_ ? y_->name_ : "NULL"));
       break;
     case ListXTest:
       log("unlock", list_x_.size());
       break;
     case ListYTest:
       log("unlock", list_y_.size());
       break;
     }
  }

  int mode_;
  MockObservable *x_;
  MockObservable *y_;
  std::list<MockObservable *> list_x_;
  std::list<MockObservable *> list_y_;
};


#endif // OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_