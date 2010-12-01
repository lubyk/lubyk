
class Signal;

/** The observer can connect member methods to slots in order
 * to receive notifications.
 */
class Observer {
public:
  Observer() {}

  ~Observer() {
    disconnect_all();
  }


private:
  friend class Signal;

  void disconnect_all() {
    ScopedWrite lock(observed_signals_);
    CTList<Signal*>:iterator it, end = observed_signals_.end();

    for(it = observed_signals_.begin(); it != end;) {
      (*it)->disconnect_observer(this);
      it = observed_signals_.erase(it);
    }
  }

  void connect_signal(Signal *sig) {
    ScopedWrite lock(observed_signals_);
    observed_signals_.push_back(sig);
  }

  void disconnect_signal(Signal *sig) {
    ScopedWrite lock(observed_signals_);
    observed_signals_.remove(sig);
  }
  /** Signals connected.
   */
  CTList<Signal*> observed_signals_;
};

class Signal {
public:
  ~Signal() {
    disconnect_all();
  }

  void connect(Observer *observer) {
    ScopedWrite lock(observers_);
    observers_.push_back(observer);
    observer->connect_signal(this);
  }

  void disconnect(Observer *observer) {
    disconnect_observer(observer);
    observer->disconnect_signal(this);
  }

private:
  friend class Observer;

  void disconnect_observer(Observer *observer) {
    ScopedWrite lock(observers_);
    observers_.remove(observer);
  }

  void disconnect_all() {
    ScopedWrite lock(observers_);
    CTList<Observer*>:iterator it, end = observers_.end();

    for(it = observers_.begin(); it != end;) {
      (*it)->disconnect_observer(this);
      it = observers_.erase(it);
    }
  }

  CTList<Observer*> observers_;
}