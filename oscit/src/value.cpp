#include "oscit/values.h"
#include <iostream>
#include <ostream>

namespace oscit {

std::ostream &operator<<(std::ostream &out_stream, const Value &val) {
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << val.r;
      break;
    case ERROR_VALUE:
      out_stream << "\"" << val.error_code() << " " << val.error_message() << "\"";
      break;
    case STRING_VALUE:
      out_stream << "\"" << val.s << "\"";
      break;
    case HASH_VALUE:
      out_stream << "{" << *val.hash_ << "}";
      break;
    case MATRIX_VALUE:
      out_stream << "\"Matrix " << val.matrix_->rows << "x" << val.matrix_->cols << "\"";
      break;
    case NIL_VALUE:
      out_stream << "Nil";
      break;
    case LIST_VALUE:
      size_t sz = val.size();
      out_stream << "[";
      for (size_t i = 0; i < sz; ++i) {
        if (i > 0) out_stream << ", ";
        out_stream << val[i];
      }
      out_stream << "]";
      break;
    default:
      ;// ????
  }
  return out_stream;
}
std::string Value::as_string() const {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  return os.str();
}
} // oscit
