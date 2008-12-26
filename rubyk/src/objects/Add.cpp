#include "rubyk.h"

//////////  Used for testing. Do not modify (Use 'Plus' object). ///////////


class Add : public Node
{
public:
  bool init ()
  {
    mValue1 = 0;
    mValue2 = 0;
    mResult = 0;
    return true;
  }
  
  ATTR_ACCESSOR(mValue1, value1)
  ATTR_ACCESSOR(mValue2, value2)
  
  virtual const Value inspect(const Value& val)  
  { 
    std::ostringstream oss;
    mResult = mValue1 + mValue2;
    oss << "<Add:" << url() << " " << mResult << ">";
    return String(oss.str());
  }
  
  // [1]
  void bang(const Value& val)
  { 
    // std::cout << mName << ".bang: mValue1 = " << val << std::endl;
    mValue1 = val;
    mResult = mValue1 + mValue2;
    
    // std::cout << "=> mResult: [" << mResult.data_id() << "/" << mResult.ref_count() << "] = " << mResult.value() << std::endl;
    send(mResult);
  }
  
  // [2]
  void adder(const Value& val)
  { 
    // std::cout << mName << ".adder: mValue2 = " << val << std::endl;
    mValue2 = val;
  }

  
private:
  Number mValue1;
  Number mValue2;
  Number mResult;
};

extern "C" void init(Planet& planet)
{
  CLASS (Add, "Add two numbers together (used for testing: only send new value on [1] change).")
  INLET (Add, bang,  NumberValue | BangValue, "Set first number to add and send.")
  INLET (Add, adder, NumberValue, "Set second number to add.")
  OUTLET(Add, sum, NumberValue, "value1 + value2")
  ACCESSOR(Add, value1, "Set first number.")
  ACCESSOR(Add, value2, "Set second number.")
}