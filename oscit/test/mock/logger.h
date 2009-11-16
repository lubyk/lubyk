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

  template<class S, class T>
  void log(const S& info1, const T& info2) {
    *stream_ << "[" << identifier_ << ": " << info1 << " " << info2 << "]";
  }

  template<class S, class T, class U>
  void log(const S& info1, const T& info2, const U& info3) {
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << "]";
  }

  template<class S, class T, class U, class V>
  void log(const S& info1, const T& info2, const U& info3, const V& info4) {
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << " " \
             << info4 << "]";
  }

private:
  std::string identifier_;
  std::ostream *stream_;
};


#endif // OSCIT_TEST_MOCK_LOGGER_H_
