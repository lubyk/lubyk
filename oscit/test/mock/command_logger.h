#ifndef OSCIT_TEST_MOCK_COMMAND_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"

class CommandLogger : public Command, protected MockLogger {
public:
  CommandLogger(std::ostringstream *stream) :
                     Command("dummy"),
                     MockLogger("dummy", stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}
  CommandLogger(const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(protocol, stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}
  CommandLogger(const char *name, const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(name, stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}


  ~CommandLogger() {
   // make sure command is halted before being destroyed
   kill();
  }

  virtual void listen() {
    log("listen");
    thread_ready();
    while (should_run()) {
      lock();
        log(".");
      unlock();
      millisleep(20);
    }
  }

  virtual void notify_observers(const char *path, const Value &val) {
    log("notify", path, val);
  }

  virtual void send(const Location &remote_endpoint, const char *path, const Value &val) {
    log("send", remote_endpoint, path, val);
  }

  virtual Object *build_remote_object(const Url &url, Value *error) {
    log("build_remote_object", url);
    if (url.location() == dummy_host_) {
      return adopt_remote_object(url.str(), new DummyObject(url.str().c_str(), url.port()));
    } else {
      error->set(BAD_REQUEST_ERROR, std::string("Unknown location '").append(url.location().name()).append("'."));
      return NULL;
    }
  }

  Object *remote_object_no_build(const std::string &url) {
    Object *res = NULL;
    remote_objects_.get(url, &res);
    return res;
  }

  const std::list<Location> &observers() const {
    return Command::observers();
  }

  Location dummy_host_;
};


#endif // OSCIT_TEST_MOCK_COMMAND_LOGGER_H_