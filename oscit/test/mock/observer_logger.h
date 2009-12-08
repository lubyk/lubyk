#ifndef OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_
#define OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_

#include "oscit/observer.h"
#include "mock/logger.h"
#include "oscit/tobservable_list.h"

class ObserverLogger : public Observable, public MockLogger {
public:
  enum TestType {
    ClbkTest,
    VarTest,
    ListXTest,
    ListYTest
  };
  ObserverLogger(const char *name, Logger *logger)
      : MockLogger(name, logger),
        mode_(ClbkTest),
        x_(NULL),
        y_(NULL),
        list_x_(this),
        list_y_(this),
        callbacks_for_event_xyz_(this) {}

  ObserverLogger(const char *name, Logger *logger, TestType mode)
      : MockLogger(name, logger),
        mode_(mode),
        x_(NULL),
        y_(NULL),
        list_x_(this),
        list_y_(this),
        callbacks_for_event_xyz_(this) {}

  virtual void observer_lock() {
    switch(mode_) {
    case ClbkTest:
      log("lock");
      break;
    case VarTest:
      log("lock", (x_ ? x_->identifier_.c_str() : "NULL"), (y_ ? y_->identifier_.c_str() : "NULL"));
      break;
    case ListXTest:
      log("lock", list_x_.size());
      break;
    case ListYTest:
      log("lock", list_y_.size());
      break;
    }
  }

  virtual void observer_unlock() {
     switch(mode_) {
     case ClbkTest:
       log("unlock");
       break;
     case VarTest:
        log("unlock", (x_ ? x_->identifier_.c_str() : "NULL"), (y_ ? y_->identifier_.c_str() : "NULL"));
       break;
     case ListXTest:
       log("unlock", list_x_.size());
       break;
     case ListYTest:
       log("unlock", list_y_.size());
       break;
     }
  }

  void set_x(ObserverLogger *x) {
    set_and_hold(&x_, x);
  }

  void set_y(ObserverLogger *y) {
    set_and_hold(&y_, y);
  }

  void push_back_x(ObserverLogger *x) {
    list_x_.push_back(x);
  }

  void push_back_y(ObserverLogger *y) {
    list_y_.push_back(y);
  }

  void remove_x(ObserverLogger *x) {
    list_x_.remove(x);
  }

  void remove_y(ObserverLogger *y) {
    list_x_.remove(y);
  }


  void my_callback(void *data) {
    const char *message = (const char *)data;
    log("my_callback", message);
  }

  void adopt_callback_for_event_xyz(Callback *callback) {
    callbacks_for_event_xyz_.adopt_callback(callback);
  }

  void event_xyz() {
    callbacks_for_event_xyz_.trigger_all();
  }

  void on_registration_callback(void *data) {
    std::string *url = (std::string *)data;
    log("on_registration_callback", *url);
  }

  class OnRegistrationCallback : public TCallback<ObserverLogger, &ObserverLogger::on_registration_callback> {
  public:
    OnRegistrationCallback(ObserverLogger *observer, const std::string &url)
        : TCallback<ObserverLogger, &ObserverLogger::on_registration_callback>(observer, new std::string(url)) {}

    OnRegistrationCallback(ObserverLogger *observer, const char *url)
        : TCallback<ObserverLogger, &ObserverLogger::on_registration_callback>(observer, new std::string(url)) {}

    virtual ~OnRegistrationCallback() {
      std::string *str = (std::string*)data_;
      delete str;
    }
  };

  int mode_;
  ObserverLogger *x_;
  ObserverLogger *y_;
  TObservableList<ObserverLogger *> list_x_;
  TObservableList<ObserverLogger *> list_y_;

private:
  CallbackList callbacks_for_event_xyz_;
};

template<class T, void(T::*Tmethod)(void *data)>
class TCallbackLogger : public Callback, public MockLogger {
public:
  TCallbackLogger(T *observer, void *data, const char *name, Logger *logger) : Callback(observer, data), MockLogger(name, logger) {}

  ~TCallbackLogger() {
    log("destroy");
  }

  virtual void trigger() {
    log("trigger");
    (((T*)observer_)->*Tmethod)(data_);
  }
};

#endif // OSCIT_TEST_MOCK_OBSERVER_LOGGER_H_