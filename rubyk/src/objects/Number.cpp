#include "rubyk.h"

class NumberObj : public Node
{
public:
  
  /** value accessor. */
  ATTR_ACCESSOR(mValue, value)
  
  // [1] set/output current value (Bang! just outputs value).
  void bang(const Value& val)
  { 
    // std::cout << mName << ".bang: mValue [" << mValue.data_id() << "/" << mValue.ref_count() << "] = " << val << std::endl;
    mValue = val;
    send(mValue);
  }
  
  // [2] set value
  void value(const Value& val)
  {
    // std::cout << mName << ".value: mValue = " << val << std::endl;
    mValue = val;
  }
  
  // #inspect
  virtual const Value inspect(const Value& val) 
  { 
    std::ostringstream oss;
    oss << "<Number:" << url() << " " << mValue << ">";
    return String(oss.str());
  }
  
private:
  Number mValue;
};

extern "C" void init(Root& root)
{
  Class * c = root.classes()->declare<NumberObj>("Number", "Store a number which can be sent again through Bang!.");
  
  INLET (  NumberObj, bang,  NumberValue, "Send current value on bang. Set and send for number.")
  OUTLET(  NumberObj, value, NumberValue, "Current value.")
  ACCESSOR(NumberObj, value, "Set/get current value.")
}