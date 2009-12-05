#ifndef OSCIT_TEST_MOCK_CALLBACK_LOGGER_H_
#define OSCIT_TEST_MOCK_CALLBACK_LOGGER_H_

#include "oscit/callback.h"
#include "oscit/callback_observer.h"
#include "mock/logger.h"

class CallbackObserverLogger : public CallbackObserver, public MockLogger {
public:
  CallbackObserverLogger(const char *name, Logger *logger) : MockLogger(name, logger) {}

  void my_callback(void *data) {
    const char *message = (const char *)data;
    log("my_callback", message);
  }
private:
  const char *name_;
};

class CallbackObservableMock {
public:
  CallbackObservableMock() {}

  void adopt_callback_for_event_xyz(Callback *callback) {
    callbacks_for_event_xyz_.adopt_callback(callback);
  }

  void event_xyz() {
    callbacks_for_event_xyz_.trigger_all();
  }

private:
  CallbackList callbacks_for_event_xyz_;
};

template<class T, void(T::*Tmethod)(void *data)>
class TCallbackLogger : public Callback, public MockLogger {
public:
  TCallbackLogger(T *observer, void *data, const char *name, Logger *logger) : Callback(observer), MockLogger(name, logger), data_(data) {}

  ~TCallbackLogger() {
    log("destroy");
  }

  virtual void trigger() {
    log("trigger");
    (((T*)observer_)->*Tmethod)(data_);
  }

private:
  void *data_;
};

#endif // OSCIT_TEST_MOCK_CALLBACK_LOGGER_H_