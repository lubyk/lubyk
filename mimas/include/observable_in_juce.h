#ifndef MIMAS_SRC_CORE_OBSERVABLE_IN_JUCE_H_
#define MIMAS_SRC_CORE_OBSERVABLE_IN_JUCE_H_
#include "oscit/observable.h"

class ObservableInJuce : public Observable {
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
#endif // MIMAS_SRC_CORE_OBSERVABLE_IN_JUCE_H_