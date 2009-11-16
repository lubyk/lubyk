#ifndef OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_
#define OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_

#include "oscit/object.h"
#include "mock/logger.h"

class DestroyLogger : public Object, protected MockLogger {
public:
  DestroyLogger(const char *name, std::ostringstream *stream) :
                Object(name),
                MockLogger(name, stream) {}

  virtual ~DestroyLogger() {
    log("destroyed");
  }
};


#endif // OSCIT_TEST_MOCK_OBJECT_DESTROY_LOGGER_H_