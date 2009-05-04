#ifndef _OSCIT_STRING_H_
#define _OSCIT_STRING_H_
#include "oscit/values.h"
#include "oscit/reference_counted.h"

namespace oscit {

/** A String is just a reference counted std::string. */
class StringData : public ReferenceCounted, public std::string
{
 public:
  explicit StringData(const char *str) : std::string(str) {}
  explicit StringData(const std::string &string) : std::string(string) {}
};

} // oscit
#endif // _OSCIT_STRING_H_