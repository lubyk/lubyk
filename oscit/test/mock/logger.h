#ifndef OSCIT_TEST_MOCK_LOGGER_H_
#define OSCIT_TEST_MOCK_LOGGER_H_

#include <sstream>

class MockLogger {
public:
  MockLogger(const char *identifier, std::ostringstream *stream) : identifier_(identifier), stream_(stream) {}

protected:
  template<class T>
  void log(const T& info) {
    *stream_ << "[" << identifier_ << ": " << info << "]";
  }

private:
  std::string identifier_;
  std::ostream *stream_;
};


#endif // OSCIT_TEST_MOCK_LOGGER_H_