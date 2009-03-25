#ifndef _LIST_H_
#define _LIST_H_
#include "oscit/values.h"

namespace oscit {

/** A List stores an array of Values. */
class List
{
public:
  explicit List(const char * typeTag = "")
  {
    typeTagStorage_ = typeTag;
    typeTag_ = typeTagStorage_.c_str();
    const char * c = typeTag_;
    while ( *c )
    {
      values_.push_back(Value(*c));
      c++;
    }
  }
  
  const char * getTypeTag() const
  {
    return typeTag_;
  }
  
  const Value& operator[](size_t pos) const
  {
    return values_[pos];
  }
  
  Value& operator[](size_t pos)
  {
    return values_[pos];
  }
private:
  std::string typeTagStorage_;
  const char * typeTag_;
  std::vector<Value> values_;
};

} // oscit
#endif // _LIST_H_