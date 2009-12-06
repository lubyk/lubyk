#ifndef OSCIT_INCLUDE_OSCIT_ON_DESTROY_CALLBACK_H_
#define OSCIT_INCLUDE_OSCIT_ON_DESTROY_NOTIFIER_H_

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
    Iterator it, end;
    end = callbacks_.end();
    for (it = callbacks_.begin(); it != end; ++it) {
      (*it)->trigger();
      delete *it;
    }
  }

  /** Adopt a new callback.
   *  The notifier becomes responsible for the callback deletion.
   */
  void adopt_callback(Callback *callback) {
    callbacks_.push_back(callback);
  }

  /** Remove callback identified by the given key.
   */
  void remove_callback(void *var, void *data) {
    std::list<Callback*>::iterator it, end;
    end = callbacks_.end();
    for (it = callbacks_.begin(); it != end; ++it) {
      if ((*it)->key_match(var, data)) {
        delete *it;
        it = callbacks_.erase(it);
      }
    }
  }

  /** Find the callback identified by the given key,
   *  trigger it and remove from list.
   */
  void trigger_and_remove(void *var, void *data) {
    std::list<Callback*>::iterator it, end;
    end = callbacks_.end();
    for (it = callbacks_.begin(); it != end; ++it) {
      if ((*it)->key_match(var, data)) {
        (*it)->trigger();
        delete *it;
        it = callbacks_.erase(it);
      }
    }
  }

private:
  std::list<Callback*> callbacks_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ON_DESTROY_NOTIFIER_H_