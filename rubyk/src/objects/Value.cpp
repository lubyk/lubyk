#include "rubyk.h"

class Number : public Node
{
public:
//  /** value accessor. */
  const Value value_accessor(const Value &val) {
    std::cout << "value_accessor(" << val << ")\n"; 
    value_ = val;
    return value_;
  }

  // [1] set/output current value (Bang! just outputs value).
  void bang(const Value &val) { 
    // std::cout << name_ << ".bang: value_ [" << value_.data_id() << "/" << value_.ref_count() << "] = " << val << std::endl;
    if (val.is_real()) value_.r = val.r;
    // std::cout << name_ << " send " << value_ << std::endl;
    send(value_);
  }

//  // [2] set value
//  void value(const Value &val)
//  {
//    // std::cout << name_ << ".value: value_ = " << val << std::endl;
//    if (val) value_.r = val->r;
//  }
//  
  
  virtual const Value inspect() {
    Value hash;
    hash.set("value", value_.r);
    return hash;
  }
  
private:
  Value value_;
};

extern "C" void init(Planet &planet) {
  CLASS(Number, "Stores a number which can be sent again through Bang!.", "value: [initial value]");
  INLET(Number, bang, FieldIO("any real", "Set and send current value."));
  ADD_METHOD(Number, "value", value_accessor, FieldIO("any real", "Set/get current value."))
  // OUTLET(Number, value, FieldIO("-"), "Current value.")
  // ACCESSOR(NumberObj, value, "Set/get current value.")
}