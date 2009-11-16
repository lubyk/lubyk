#ifndef OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"

class CommandActionLogger : public Command, protected MockLogger {
public:
  CommandActionLogger(const char *protocol,std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(protocol, stream) {}
  CommandActionLogger(const char *name, const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(name, stream) {}


  virtual void listen() {
    log("listen");
    thread_ready();
    millisleep(5000); // will be interupted
  }

  virtual void notify_observers(const char *path, const Value &val) {
    log(path, val);
  }

  virtual Object *build_remote_object(const Url &remote_url, Value *error) {
    log(remote_url);
    return NULL;
  }
};


#endif // OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_