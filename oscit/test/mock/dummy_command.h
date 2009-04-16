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
  
  virtual BaseObject *build_remote_object(const Url &url) {
    if (url.host() == "dummy.host") {
      return remote_objects_->adopt(new DummyObject(url.path().c_str(), url.port()));
    } else {
      return NULL;
    }
  }
  
  BaseObject *remote_object_no_build(const std::string &url) {
    return remote_objects_->object_at(url);
  }
  
  std::string * string_;
};

#endif // _DUMMY_COMMAND_H_