#ifndef _OSCIT_LIST_VALUE_H_
#define _OSCIT_LIST_VALUE_H_
#include "oscit/value.h"
#include "oscit/list.h"

namespace oscit {

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages.*/
class ListValue : public Value
{ 
 public:
  explicit ListValue(const TypeTag &type_tag) : Value(type_tag) {}
  explicit ListValue(const char *type_tag) : Value(TypeTag(type_tag)) {}
  explicit ListValue() {
    set_type(LIST_VALUE);
  }
};

} // oscit
#endif // _OSCIT_LIST_VALUE_H_