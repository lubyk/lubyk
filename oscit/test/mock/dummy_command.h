#ifndef _DUMMY_COMMAND_H_
#define _DUMMY_COMMAND_H_
#include "oscit/base_command.h"

struct DummyCommand : public BaseCommand
{
 public:
  DummyCommand(std::string *string) : BaseCommand("dummy"), string_(string) {}
  DummyCommand(std::string *string, const char *protocol) : BaseCommand(protocol), string_(string) {}
  
  void do_listen() {
    while (should_run()) {
      string_->append(".");
      microsleep(10);
    }
  }
  
  virtual BaseObject *build_remote_object(const Url &url, Value *error) {
    if (url.host() == "dummy.host") {
      return adopt_remote_object(url.str(), new DummyObject(url.str().c_str(), url.port()));
    } else {
      error->set(BAD_REQUEST_ERROR, std::string("Unknown host '").append(url.host()).append("'."));
      return NULL;
    }
  }
  
  BaseObject *remote_object_no_build(const std::string &url) {
    BaseObject *res = NULL;
    remote_objects_.get(url, &res);
    return res;
  }
  
  std::string * string_;
};

#endif // _DUMMY_COMMAND_H_