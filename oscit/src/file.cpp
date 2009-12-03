#include "oscit/file.h"

#include <string>
#include <sstream>
#include <fstream>    // file io


namespace oscit {

bool File::write(const std::string &content) {
  // FIXME: atomic !!
  std::ofstream out(path_.c_str(), std::ios::out);
    if (out.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not write to file '").append(path_).append("'."));
      return false;
    }
    out << content;
  out.close();
  return true;
}

std::string File::read() {
  std::ostringstream oss;
  // FIXME: atomic !!
  std::ifstream in(path_.c_str(), std::ios::in);
    if (in.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not read file '").append(path_).append("'."));
      return std::string("");
    }
    oss << in.rdbuf();
  in.close();
  return oss.str();
}

bool File::append(const std::string& data) {
  // FIXME: atomic !!
  std::ofstream out(path_.c_str(), std::ios_base::app);
    if (out.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not append to file '").append(path_).append("'."));
      return false;
    }
    out << data;
  out.close();
  return true;
}

} // oscit
