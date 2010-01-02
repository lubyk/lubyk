#include "mimas.h"
#include "m_view_proxy.h"

#include <string>
#include <iostream>

#include "mimas_window_content.h"
#include "m_device_view.h"

/** This class responds to the "update" method in a view and is
 * responsible for building/updating the view accordingly.
 */
class MUpdateViewProxy : public MObjectProxy {
public:
  MUpdateViewProxy(MViewProxy *view_proxy, const Value &type)
      : MObjectProxy(view_proxy->mimas(), std::string("update"), type),
        view_proxy_(view_proxy) {}

  /** "update" method inside the view.
   */
  virtual void value_changed(const Value &val) {
    MessageManagerLock mml;
    view_proxy_->update_view(val);
  }

private:
  MViewProxy *view_proxy_;
};


// ==================================== MViewProxy =============

MViewProxy::MViewProxy(MimasWindowContent *mimas, const std::string &name, const Value &type)
    : MObjectProxy(mimas, name, type),
      view_(NULL) {}


MViewProxy::~MViewProxy() {
  MessageManagerLock mml;
  if (view_) delete view_;
}

void MViewProxy::adopted() {
  ObjectProxy::adopted();
  if (root_proxy_) {
    sync_children();

    MessageManagerLock mml;
    mimas_->workspace()->addAndMakeVisible(view());
  }
}

Object *MViewProxy::build_child(const std::string &name, const Value &type, Value *error) {
  if (name == "update") {
    return adopt(new MUpdateViewProxy(this, type));
  } else {
    return NULL;
  }
}

// widget callback
void MViewProxy::update_remote(const Value& hash) {
  if (update_view_proxy_) {
    update_view_proxy_->set_value(hash);
  } else {
    Value def(view_definition_.to_json()); // copy
    def.deep_merge(hash);
    set_value(Value(def.to_json()));
  }
}

/** Build or update the widgets on a view from the view definitions.
 */
void MViewProxy::update_view(const Value &def) {
  if (!def.is_hash()) {
    std::cerr << "'" << url() << "' cannot update view: parameter is not a Hash: " << def << "\n";
    assert(false);
    return;
  }

  std::cout << "UPDATING FROM\n" << def << "\n";

  if (!view_) {
    view_ = new MDeviceView(this, root_proxy_->remote_location().name());
  }

  view_->update(def);

  if (&def != &view_definition_) {
    view_definition_.deep_merge(def);
  }
}


