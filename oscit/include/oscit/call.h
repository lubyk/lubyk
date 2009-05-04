#ifndef OSCIT_INCLUDE_OSCIT_CALL_H_
#define OSCIT_INCLUDE_OSCIT_CALL_H_
#include <string>
#include "oscit/root.h"

namespace oscit {
  
/** This is a simple struct to store a call (url and parameter). */
struct Call
{
  Call(const std::string &url, const Value &param) : url_(url), param_(param) {}
  
  const Value trigger(Root *root) {
    return root->call(url_, param_);
  }
  
  std::string url_;
  Value param_;
};
} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_CALL_H_
