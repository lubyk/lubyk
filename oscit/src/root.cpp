#include "oscit/root.h"

// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"
#include "oscit/list_meta_method.h"
#include "oscit/list_with_type_meta_method.h"
#include "oscit/tree_meta_method.h"

#include "oscit/callback_list.h"

namespace oscit {

void Root::clear() {
  while (!commands_.empty()) {
    Command *command = commands_.front();
    command->kill();
    command->set_root(NULL); // avoid call to unregister_command in ~Command

    delete command;
    commands_.pop_front();
  }
  clear_on_register_callbacks();
  this->Object::clear();
}

void Root::clear_on_register_callbacks() {
  std::list<std::string>::iterator it, end = callbacks_on_register_.end();
  CallbackList *callbacks;

  for (it = callbacks_on_register_.begin(); it != end; ++it) {
    if (callbacks_on_register_.get(*it, &callbacks)) {
      delete callbacks;
    }
  }
}

void Root::clear_on_register_callbacks(const std::string &url) {
  CallbackList *callbacks;

  if (callbacks_on_register_.get(url, &callbacks)) {
    delete callbacks;
    callbacks_on_register_.remove(url);
  }
}

void Root::init(bool should_build_meta) {
  url_ = "";
  set_root(this);

  if (should_build_meta) {
    // build meta methods
    adopt(new ErrorMetaMethod(Url(ERROR_PATH).name()));
    adopt(new InfoMetaMethod(Url(INFO_PATH).name()));
    adopt(new ListMetaMethod(Url(LIST_PATH).name()));
    adopt(new ListWithTypeMetaMethod(Url(LIST_WITH_TYPE_PATH).name()));
    adopt(new TypeMetaMethod(Url(TYPE_PATH).name()));
    adopt(new TreeMetaMethod(Url(TREE_PATH).name()));
  }
}

void Root::adopt_callback_on_register(const std::string &url, Callback *callback) {
  CallbackList *callbacks;
  if (!callbacks_on_register_.get(url, &callbacks)) {
    callbacks = new CallbackList(this);
    callbacks_on_register_.set(url, callbacks);
  }
  callbacks->adopt_callback(callback);
}
} // oscit
