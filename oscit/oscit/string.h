#ifndef _OSCIT_STRING_H_
#define _OSCIT_STRING_H_
#include "oscit/values.h"

namespace oscit {

/** A String is just a reference counted std::string. */
struct String
{
 public:
  explicit String() : ref_count_(1) {}
  
  explicit String(const char *str) : str_(str), ref_count_(1) {}
  
  explicit String(const std::string &str) : str_(str), ref_count_(1) {}
  
  std::string str_;
  size_t ref_count_;
};

} // oscit
#endif // _OSCIT_STRING_H_