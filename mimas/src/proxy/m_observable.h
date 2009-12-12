#ifndef MIMAS_SRC_PROXY_M_OBSERVABLE_H_
#define MIMAS_SRC_PROXY_M_OBSERVABLE_H_
#include "oscit/observable.h"

class MObservable : public Observable {
public:
  virtual void observer_lock() {
    mml_ = new MessageManagerLock();
  }

  virtual void observer_unlock() {
    delete mml_;
  }
private:
  MessageManagerLock *mml_;
};
#endif // MIMAS_SRC_PROXY_M_OBSERVABLE_H_