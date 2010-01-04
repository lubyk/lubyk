/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef OSCIT_INCLUDE_OSCIT_URL_H_
#define OSCIT_INCLUDE_OSCIT_URL_H_
#include <string>
#include <ostream>

#include "oscit/location.h"

namespace oscit {

class ZeroConfBrowser;

/** The Url is used to access remote locations by wrapping protocol, ip, port and other data.
 *
 * TODO: keep only a reference to the location in case we need this information very often...
 */
class Url
{
 public:
  Url(const unsigned long ip, const uint port, const char *path) : path_(path) {
    location_.protocol_ = DEFAULT_PROTOCOL;
    location_.reference_by_hostname_ = true;
    location_.ip_   = ip;
    location_.port_ = port;
    location_.name_ = Location::name_from_ip(ip);
    rebuild_full_url();
  }

  Url(const unsigned long ip, const uint port, const std::string &path) : path_(path) {
    location_.protocol_ = DEFAULT_PROTOCOL;
    location_.reference_by_hostname_ = true;
    location_.ip_   = ip;
    location_.port_ = port;
    location_.name_ = Location::name_from_ip(ip);
    rebuild_full_url();
  }

  Url(const Location &location, const std::string &path) : location_(location), path_(path) {
    rebuild_full_url();
  }

  Url(const Location *location, const std::string &path) : path_(path) {
    if (location) location_ = *location;
    rebuild_full_url();
  }

  explicit Url(const std::string &string) {
    parse(string.c_str());
  }

  explicit Url(const char *string) {
    parse(string);
  }

  Url() {}

  const std::string &str() const { return full_url_; }

  const Location &location() const { return location_; }

  const std::string &protocol() const { return location_.protocol_; }

  bool has_hostname() const { return location_.reference_by_hostname_; }

  bool has_service_name() const { return !location_.reference_by_hostname_; }

  bool is_meta() const {
    return path_.compare(0, 2, "/.") == 0;
  }

  const std::string &hostname() const { return location_.name_; }

  const std::string &service_name() const { return location_.name_; }

  void resolve_with(const ZeroConfBrowser *browser) {
    location_.resolve_with(browser);
  }

  unsigned long ip() const { return location_.ip_; }

  uint port() const { return location_.port_; }

  const std::string &path() const { return path_; }

  /** Return the name of the last element in the url's path.
   */
  const std::string name() {
    size_t pos = path_.rfind("/");
    if (pos != std::string::npos) {
      return path_.substr(pos + 1);
    } else {
      return path_;
    }
  }

  /** Get the url above the current url. Returns true on success
   * or false if the current url is already a top-level url.
   */
  bool get_parent_url(Url *parent_url) const {
    size_t pos = path_.rfind("/");
    if (pos != std::string::npos) {
      parent_url->location_ = location_;
      parent_url->path_ = path_.substr(0, pos);
      parent_url->rebuild_full_url();
      return true;
    } else {
      return false;
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

  bool operator==(const Url &other) {
    return full_url_ == other.full_url_;
  }

 private:
  friend std::ostream &operator<<(std::ostream &out_stream, const Url &url);
  void parse(const char *string);

  void rebuild_full_url();

  void clear() {
    location_.clear();
    path_ = "";
  }

  /** FIXME: document why we this string...
   */
  std::string full_url_;

  /** Host, ip and other information to reach a remote location.
   *  This can refer to the *target location* if the url is going out
   *  or to the *source location* when the url is received.
   */
  Location location_;

  /** Path to method on the called location (callee).
   */
  std::string path_;
};

std::ostream &operator<<(std::ostream &out_stream, const Url &url);

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_URL_H_
