#ifndef _OSCIT_URL_H_
#define _OSCIT_URL_H_
#include <string>
#include <ostream>

typedef unsigned int uint;

namespace oscit {

/** Url is a parsed string with easy access to protocol, host, port, path, etc. */
class Url
{
 public:
  explicit Url(const std::string &string) : full_url_(string), port_(0) {
    parse(string.c_str());
  }
  
  explicit Url(const char *string) : full_url_(string), port_(0) {
    parse(string);
  }
  
  const std::string &str() const { return full_url_; }
  
  const std::string &protocol() const { return protocol_; }
  
  const std::string &host() const { return host_; }
  
  uint port() const { return port_; }
  
  const std::string &path() const { return path_; }
  
 private:
  friend std::ostream &operator<<(std::ostream &out_stream, const Url &url);
  void parse(const char *string);
  
  void rebuild_full_url();
  
  void clear() {
    protocol_ = "";
    host_     = "";
    port_     = 0;
    path_     = "";
  }
  
  std::string full_url_;
  std::string protocol_;
  std::string host_;
  uint        port_;
  std::string path_;
};

std::ostream &operator<<(std::ostream &out_stream, const Url &url);

} // oscit

#endif // _OSCIT_URL_H_