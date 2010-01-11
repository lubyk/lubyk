#include <iostream>
#include <sstream>
#include <list>


struct Named {
  Named(const char *name) : name_(name) {}
  const char *name_;
};

class Observer;

class Observable : public Named {
public:
  Observable(const char *name) : Named(name) {}

protected:
  friend class Observer;


  class Callback {
  public:
    virtual ~Callback() {}
    virtual void trigger() = 0;
    virtual bool key_match(void *key) = 0;
    virtual std::string inspect() = 0;
  };

  typedef std::list<Callback*> CallbackList;

  template<class T, void(T::*Tmethod)(void *data)>
  class TCallback : public Callback, public Named {
  public:
    TCallback(const char *type, T *owner, void *data) : Named(type), owner_(owner), data_(data) {}

    ~TCallback() {}
    virtual void trigger() {
      // std::cout << "    trigger: " << inspect() << "\n";
      (owner_->*Tmethod)(data_);
    }

    bool key_match(void *key) {
      return data_ == key;
    }

    virtual std::string inspect() {
      std::ostringstream out;
      out << "callback: " << owner_->name_ << "->" << name_ << "(" << data_ << ")";
      return out.str();
    }

  private:
    T *owner_;
    void *data_;
  };


  class OnDestroyNotifier {
  public:
    ~OnDestroyNotifier() {
      CallbackList::iterator it, end;
      end = callbacks_.end();
      for (it = callbacks_.begin(); it != end; ++it) {
        (*it)->trigger();
        delete *it;
      }
    }

    void adopt_callback(Callback *callback) {
      callbacks_.push_back(callback);
    }

    void remove_callback(void *key) {
      CallbackList::iterator it, end;
      end = callbacks_.end();
      for (it = callbacks_.begin(); it != end; ++it) {
        if ((*it)->key_match(key)) {
          std::cout << "    removing: " << (*it)->inspect() << "\n";
          delete *it;
          it = callbacks_.erase(it);
        }
      }
    }

    void trigger_and_remove(void *key) {
      CallbackList::iterator it, end;
      end = callbacks_.end();
      for (it = callbacks_.begin(); it != end; ++it) {
        if ((*it)->key_match(key)) {
          std::cout << "    trigger & remove: " << (*it)->inspect() << "\n";
          (*it)->trigger();
          delete *it;
          it = callbacks_.erase(it);
        }
      }
    }

  private:
    CallbackList callbacks_;
  };

  void on_destroy(Callback *callback) {
    std::cout << name_ << "->on_destroy(" << callback->inspect() << ")\n";
    destruction_notifier_.adopt_callback(callback);
  }

  void disable_on_destroy(void *key) {
    std::cout << name_ << "->disable_on_destroy(" << key << ")\n";
    destruction_notifier_.remove_callback(key);
  }
private:
  // Observable ivars
  OnDestroyNotifier destruction_notifier_;
};


class Observer : public Named {
public:
  Observer(const char *name) : Named(name) {}

  virtual void clear_ivar(void *ivar_ptr) {
    Observable **ivar = (Observable **)ivar_ptr;
    std::cout << name_ << "->clear_ivar(" << *ivar << ")\n";
    release(*ivar, false);
    ivar = NULL;
  }

  template<class T>
  void set_and_hold(T *&ivar, T *target) {
    std::cout << name_ << "->set_and_hold(" << &ivar << ", " << target << ")\n";
    if (ivar) {
      // stop observing previous value
      release(ivar, true);
    }
    ivar = target;
    target->on_destroy(new Observable::TCallback<Observer, &Observer::clear_ivar>("clear_ivar", this, &ivar));
    // if we die before the target, it should remove us from the notification list
    destruction_notifier_.adopt_callback(
      new Observable::TCallback<Observable, &Observable::disable_on_destroy>("disable_on_destroy", target, &ivar)
    );
  }

  template<class T>
  void release(T *&ivar, bool notify) {
    std::cout << name_ << "->release " << ivar << " {\n";
    if (notify)
      destruction_notifier_.trigger_and_remove(&ivar);
    else
      destruction_notifier_.remove_callback(&ivar);
    std::cout << "}\n";
  }

private:
  Observable::OnDestroyNotifier destruction_notifier_;
};

class B : public Observable {
public:
  B(const char *name) : Observable(name) {}
};

class A : protected Observer, public Observable {
public:
  A(const char *name) : Observer(name), Observable(name), x_(NULL), y_(NULL) {}

  void set_x(Observable *target) {
    set_and_hold(x_, target);
  }

  void set_y(Observable *target) {
    set_and_hold(y_, target);
  }

  void *x_ptr() { return &x_; }
  void *y_ptr() { return &y_;}
  void inspect() {
    std::cout << name_ << ": x_ = " << x_ << ", y_ = " << y_ << "\n";
  }
private:
  const char *name_;
  Observable *x_;
  Observable *y_;
};

#define DO(x) std::cout << #x << " {\n";x;std::cout << "}\n\n";

int main() {
  A *a = new A("a");
  B *b = new B("b");
  A *c = new A("c");

  std::cout << "a: " << a << "\n";
  std::cout << "x: " << a->x_ptr() << "\n";
  std::cout << "y: " << a->y_ptr() << "\n\n";
  std::cout << "b: " << b << "\n";
  std::cout << "c: " << c << "\n";

  DO(a->set_x(b));
  DO(a->set_y(b));

  DO(a->set_x(c));
  DO(c->set_x(a));

  DO(delete c);
  a->inspect();
  DO(delete a);
  DO(delete b);
}
