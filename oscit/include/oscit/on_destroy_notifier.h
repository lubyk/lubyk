#ifndef OSCIT_INCLUDE_OSCIT_ON_DESTROY_NOTIFIER_H_
#define OSCIT_INCLUDE_OSCIT_ON_DESTROY_NOTIFIER_H_

#include "oscit/callback_list.h"

namespace oscit {

/** Holds a list of callbacks that will be triggered on destruction. This is used
 * as instance variable to make sure the callbacks are triggered when the object
 * containing the OnDestroyNotifier is destroyed.
 */
class OnDestroyNotifier : public CallbackList {
public:
  /** On destruction, the notifier triggers all its callbacks.
   */
  ~OnDestroyNotifier() {
    Iterator it, end = callbacks_.end();
    for (it = callbacks_.begin(); it != end; ++it) {
      (*it)->trigger();
      delete *it;
    }
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ON_DESTROY_NOTIFIER_H_