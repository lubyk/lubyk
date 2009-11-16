#ifndef OSCIT_TEST_MOCK_OBJECT_LOGGER_H_
#define OSCIT_TEST_MOCK_OBJECT_LOGGER_H_

#include "oscit/object.h"
#include "mock/logger.h"

class ObjectLogger : public Object, protected MockLogger {
public:
  ObjectLogger(const char *name, std::ostringstream *stream) :
                Object(name),
                MockLogger(name, stream) {}

  virtual ~ObjectLogger() {
    log("destroyed");
  }
};


#endif // OSCIT_TEST_MOCK_OBJECT_LOGGER_H_