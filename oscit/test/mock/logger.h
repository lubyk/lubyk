#ifndef OSCIT_TEST_MOCK_LOGGER_H_
#define OSCIT_TEST_MOCK_LOGGER_H_

#include <sstream>

class Logger : public std::ostringstream {
public:
  Logger() : std::ostringstream(std::ostringstream::out) {}
};

class MockLogger {
public:
  MockLogger(const char *identifier, std::ostream *stream) : identifier_(identifier), stream_(stream) {}

protected:
  template<class T>
  void log(const T& info) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " << info << "]";
  }

  template<class S, class T>
  void log(const S& info1, const T& info2) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " << info1 << " " << info2 << "]";
  }

  template<class S, class T, class U>
  void log(const S& info1, const T& info2, const U& info3) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << "]";
  }

  template<class S, class T, class U, class V>
  void log(const S& info1, const T& info2, const U& info3, const V& info4) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << " " \
             << info4 << "]";
  }

  void set_stream(std::ostream *stream) {
    stream_ = stream;
  }

private:
  std::string identifier_;
  std::ostream *stream_;
};


#endif // OSCIT_TEST_MOCK_LOGGER_H_
