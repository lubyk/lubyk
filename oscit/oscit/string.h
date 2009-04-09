#ifndef _OSCIT_STRING_H_
#define _OSCIT_STRING_H_
#include "oscit/values.h"
#include "oscit/reference_counted.h"

namespace oscit {

/** A String is just a reference counted std::string. */
class String : public ReferenceCounted, public std::string
{
 public:
  explicit String(const char *str) : std::string(str) {}
  explicit String(const std::string &string) : std::string(string) {}
};

} // oscit
#endif // _OSCIT_STRING_H_