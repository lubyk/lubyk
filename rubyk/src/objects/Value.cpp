#include "rubyk.h"

class ValueNode : public Node
{
public:
  
  // [1] set/get value, send value
  const Value value(const Value &val) {
    if (!val.is_nil()) {
      value_ = val;
    }
    send(value_);
    return value_;
  }
  
  virtual void inspect(Value *hash) const {
    hash->set("value", value_);
  }
  
private:
  Value value_;
};

extern "C" void init(Planet &planet) {
  Class * c = planet.classes()->declare<ValueNode>("Value", "Stores a number which can be sent again through Bang!.", "value: [initial value]");
  METHOD(ValueNode, value, AnyIO("Set/get current value."))
  OUTLET(ValueNode, value, AnyIO("Send the current value out."))
}