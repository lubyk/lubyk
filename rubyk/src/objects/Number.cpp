#include "rubyk.h"

class NumberObj : public Node
{
public:
  
  /** value accessor. */
  ATTR_ACCESSOR(mValue, value)
  
  // [1] set/output current value (Bang! just outputs value).
  void bang(const Value val)
  { 
    // std::cout << mName << ".bang: mValue [" << mValue.data_id() << "/" << mValue.ref_count() << "] = " << val << std::endl;
    if (val) mValue.r = val->r;
    // std::cout << mName << " send " << mValue << std::endl;
    send(mValue);
  }
  
  // [2] set value
  void value(const Value val)
  {
    // std::cout << mName << ".value: mValue = " << val << std::endl;
    if (val) mValue.r = val->r;
  }
  
  // #inspect
  virtual const Value inspect(const Value val) 
  { 
    std::ostringstream oss;
    oss << "<Number:" << url() << " " << mValue.r << ">";
    return String(oss.str());
  }
  
private:
  Value mValue;
};

extern "C" void init(Planet& planet)
{
  Class * c = planet.classes()->declare<NumberObj>("Number", "Store a number which can be sent again through Bang!.");
  
  INLET (  NumberObj, bang,  RealValue, "Send current value on bang. Set and send for number.")
  OUTLET(  NumberObj, value, RealValue, "Current value.")
  ACCESSOR(NumberObj, value, "Set/get current value.")
}