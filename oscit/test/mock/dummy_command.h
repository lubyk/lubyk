#ifndef _DUMMY_COMMAND_H_
#define _DUMMY_COMMAND_H_
#include "oscit/command.h"
#include "mock/dummy_object.h"

struct DummyCommand : public Command
{
 public:
  DummyCommand(std::string *string) : Command("dummy"), string_(string) {}
  DummyCommand(std::string *string, const char *protocol) : Command(protocol), string_(string) {}

  ~DummyCommand() {
    // make sure command is halted before being destroyed
    kill();
  }

  void do_listen() {
    while (should_run()) {
      string_->append(".");
      unlock();
        millisleep(20);
      lock();
    }
  }

  virtual Object *build_remote_object(const Url &url, Value *error) {
    if (url.host() == "dummy.host") {
      return adopt_remote_object(url.str(), new DummyObject(url.str().c_str(), url.port()));
    } else {
      error->set(BAD_REQUEST_ERROR, std::string("Unknown host '").append(url.host()).append("'."));
      return NULL;
    }
  }

  Object *remote_object_no_build(const std::string &url) {
    Object *res = NULL;
    remote_objects_.get(url, &res);
    return res;
  }

  std::string * string_;
};

#endif // _DUMMY_COMMAND_H_
