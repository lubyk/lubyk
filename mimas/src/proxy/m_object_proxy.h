#ifndef MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_
#define MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_

#include "oscit/object_proxy.h"
#include "oscit/tobservable_list.h"

#include "object_proxy_tree_item.h"
#include "m_range_widget.h"

class MObjectProxy : public ObjectProxy {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.MObjectProxy")

  MObjectProxy(const std::string &name, const Value &type)
      : ObjectProxy(name, type),
        range_widgets_(this) {
    set_and_hold(&tree_view_item_, new ObjectProxyTreeItem(this));
  }

  ObjectProxyTreeItem *object_proxy_view() { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

  virtual void observer_lock() {
    mml_ = new MessageManagerLock();
  }

  virtual void observer_unlock() {
    delete mml_;
  }

  virtual void value_changed() {
    std::cout << url() << " value_changed: " << value_ << "\n";
    // update real value slider
    MessageManagerLock mml;
    String remote_value("RemoteValue");
    if (value_.is_real()) {
      std::list<MRangeWidget*>::iterator it, end = range_widgets_.end();
      for (it = range_widgets_.begin(); it != end; ++it) {
        (*it)->set_remote_value(value_.r);
        // FIXME: shouldn't this logic live in the range widget thing ?
        if (!(*it)->is_dragged()) {
          // no dragging
          if ((*it)->last_drag() + (3 * latency_) < time_ref_.elapsed()) {
            // we stopped dragging long ago, move real.
            (*it)->set_value(value_.r);
          }
        }
      }
    }
  }

  /** When we finally know our type, enable the views.
   */
  virtual void type_changed();

  void connect(MRangeWidget *widget);

private:
  MessageManagerLock *mml_;
  TObservableList<MRangeWidget *> range_widgets_;
  ObjectProxyTreeItem *tree_view_item_;
};

#endif // MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_