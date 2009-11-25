#ifndef OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_
#define OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_

#include <list>

namespace oscit {

class Observer;

/** This class should be used with the Observer class. It holds a list of callbacks to trigger on
 * destruction (to invalidate the references pointing to instances of this class).
 */
class Observable {
private:
  friend class Observer;

  class Callback {
  public:
    virtual ~Callback() {}
    virtual void trigger() = 0;
    virtual bool key_match(void *key) = 0;
  };

  template<class T, void(T::*Tmethod)(void *data)>
  class TCallback : public Callback {
  public:
    TCallback(T *owner, void *data) : owner_(owner), data_(data) {}

    ~TCallback() {}
    virtual void trigger() {
      (owner_->*Tmethod)(data_);
    }

    bool key_match(void *key) {
      return data_ == key;
    }

  private:
    T *owner_;
    void *data_;
  };

  /** Holds a list of callbacks that will be triggered on destruction. This is used
   * as instance variable to make sure the callbacks are triggered when the object
   * containing the OnDestroyNotifier is destroyed.
   */
  class OnDestroyNotifier {
  public:
    /** On destruction, the notifier triggers all its callbacks.
     */
    ~OnDestroyNotifier() {
      std::list<Callback*>::iterator it, end;
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
    void remove_callback(void *key) {
      std::list<Callback*>::iterator it, end;
      end = callbacks_.end();
      for (it = callbacks_.begin(); it != end; ++it) {
        if ((*it)->key_match(key)) {
          delete *it;
          it = callbacks_.erase(it);
        }
      }
    }

    /** Find the callback identified by the given key,
     *  trigger it and remove from list.
     */
    void trigger_and_remove(void *key) {
      std::list<Callback*>::iterator it, end;
      end = callbacks_.end();
      for (it = callbacks_.begin(); it != end; ++it) {
        if ((*it)->key_match(key)) {
          (*it)->trigger();
          delete *it;
          it = callbacks_.erase(it);
        }
      }
    }

  private:
    std::list<Callback*> callbacks_;
  };

  void on_destroy(Callback *callback) {
    destruction_notifier_.adopt_callback(callback);
  }

  void disable_on_destroy(void *key) {
    destruction_notifier_.remove_callback(key);
  }

  // Observable ivars
  OnDestroyNotifier destruction_notifier_;

};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_