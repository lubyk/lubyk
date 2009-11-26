#ifndef OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_
#define OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"

class OscCommandLogger : public OscCommand, protected MockLogger {
public:
  OscCommandLogger(uint port, const char *name, std::ostringstream *stream) :
                     OscCommand(port),
                     MockLogger(name, stream) {}


  virtual void listen() {
    log("listen");
    OscCommand::listen();
  }

  virtual void notify_observers(const char *path, const Value &val) {
    log("notify_observers", path, val);
    OscCommand::notify_observers(path, val);
  }

  virtual void send(const Location &remote_endpoint, const char *path, const Value &val) {
    log("send", remote_endpoint, path, val);
    OscCommand::send(remote_endpoint, path, val);
  }

  virtual void receive(const Url &url, const Value &val) {
    log("receive", url, val);
    replies_ << val << "\n";
    Command::receive(url, val);
  }

  void clear_reply() {
    replies_.str("");
  }

  const std::string reply() {
    return replies_.str();
  }

private:
  Logger replies_;
};


#endif // OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_