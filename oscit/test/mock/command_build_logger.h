#ifndef OSCIT_TEST_MOCK_COMMAND_BUILD_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_BUILD_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"

class CommandBuildLogger : public Command, protected MockLogger {
public:
  CommandBuildLogger(const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(protocol, stream) {}

  virtual Object *build_remote_object(const Url &remote_url, Value *error) {
    log(remote_url);
    return NULL;
  }

  virtual void listen() {}
};


#endif // OSCIT_TEST_MOCK_COMMAND_BUILD_LOGGER_H_