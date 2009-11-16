#ifndef OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_
#define OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_

#include "oscit/object.h"
#include "mock/logger.h"

class ObjectActionLogger : public Object, protected MockLogger {
public:
  ObjectActionLogger(const char *name, std::ostringstream *stream) :
                Object(name),
                MockLogger(name, stream) {}

  virtual ~ObjectActionLogger() {
    log("destroyed");
  }
};


#endif // OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_