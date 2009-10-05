#ifndef OSCIT_INCLUDE_OSCIT_URL_H_
#define OSCIT_INCLUDE_OSCIT_URL_H_
#include <string>
#include <ostream>

#include "oscit/location.h"

namespace oscit {

/** Url is a parsed string with easy access to protocol, host, port, path, etc.
 *
 * TODO: could we avoid rebuilding 'callee' and 'caller' all the time and just store
 * a reference ?
 */
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

  const std::string name() {
    size_t pos = path_.rfind("/");
    if (pos != std::string::npos) {
      return path_.substr(pos + 1);
    } else {
      return path_;
    }
  }

  Url &set(const char *str) {
    clear();
    parse(str);
    return *this;
  }

  void set(const std::string &str) {
    clear();
    parse(str.c_str());
  }

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

  /** FIXME: document why we need all these strings...
   */
  std::string full_url_;

  /** Path to method on the called location (callee).
   */
  std::string path_;

  /** Location of the receiver of this url. If this value is NULL, the
   *  receiver is on the current tree (local call).
   */
  Location    *callee_;

  /** Information on the source of a call. If this value is NULL, the
   *  caller is on the current tree (local call).
   */
  Location    *caller_;
};

std::ostream &operator<<(std::ostream &out_stream, const Url &url);

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_URL_H_
