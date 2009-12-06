#ifndef OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_
#define OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_

#include <list>

#include "oscit/observer.h"
#include "oscit/on_destroy_notifier.h"

namespace oscit {

class Callback;

/** This class should be used with the Observer class. It holds a list of callbacks to trigger on
 * destruction (to invalidate the references pointing to instances of this class).
 */
class Observable : public Observer {
public:
  Observable() : destruction_notifier_(this) {}

  void adopt_callback_on_destroy(Callback *callback) {
    destruction_notifier_.adopt_callback(callback);
  }

private:
  /** List of callbacks to trigger on destruction.
   */
  OnDestroyNotifier destruction_notifier_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_