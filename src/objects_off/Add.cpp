#include "rubyk.h"

//////////  Used for testing. Do not modify (Use 'Plus' object). ///////////


class Add : public Node
{
public:
  bool init ()
  {
    value_1 = 0;
    value_2 = 0;
    mResult = 0;
    return true;
  }
  
  ATTR_ACCESSOR(value_1, value1)
  ATTR_ACCESSOR(value_2, value2)
  
  virtual const Value inspect(const Value &val)  
  { 
    std::ostringstream oss;
    mResult = value_1 + value_2;
    oss << "<Add:" << url() << " " << mResult << ">";
    return String(oss.str());
  }
  
  // [1]
  void bang(const Value &val)
  { 
    // std::cout << name_ << ".bang: value_1 = " << val << std::endl;
    value_1 = val;
    mResult = value_1 + value_2;
    
    // std::cout << "=> mResult: [" << mResult.data_id() << "/" << mResult.ref_count() << "] = " << mResult.value() << std::endl;
    send(mResult);
  }
  
  // [2]
  void adder(const Value &val)
  { 
    // std::cout << name_ << ".adder: value_2 = " << val << std::endl;
    value_2 = val;
  }

  
private:
  Number value_1;
  Number value_2;
  Number mResult;
};

extern "C" void init(Planet &planet) {
  CLASS (Add, "Add two numbers together (used for testing: only send new value on [1] change).")
  INLET (Add, bang,  NumberValue | BangValue, "Set first number to add and send.")
  INLET (Add, adder, NumberValue, "Set second number to add.")
  OUTLET(Add, sum, NumberValue, "value1 + value2")
  ACCESSOR(Add, value1, "Set first number.")
  ACCESSOR(Add, value2, "Set second number.")
}