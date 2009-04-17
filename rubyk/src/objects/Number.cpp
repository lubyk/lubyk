#include "rubyk.h"

class Number : public Node
{
public:
  
//  /** value accessor. */
//  ATTR_ACCESSOR(value_, value)

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
//  // #inspect
//  virtual const Value inspect(const Value &val) 
//  { 
//    std::ostringstream oss;
//    oss << "<Number:" << url() << " " << value_.r << ">";
//    return String(oss.str());
//  }
  
private:
  Value value_;
};

extern "C" void init(Planet &planet) {
  CLASS(Number, "Store a number which can be sent again through Bang!.", "value: [initial value]");
  INLET(Number, bang, FieldInput("-"), "Set and send current value.");
  // OUTLET(Number, value, FieldInput("-"), "Current value.")
  // ACCESSOR(NumberObj, value, "Set/get current value.")
}