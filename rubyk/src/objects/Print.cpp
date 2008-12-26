#include "rubyk.h"

class Print : public Node
{
public:
  
  // [1]
  void bang(const Value& val)
  { 
    // std::cout << "received " << val << std::endl;
    notify(H("print"), val);
  }
  
  virtual const Value inspect(const Value& val) 
  { 
    return String("<Print:").append(url()).append(" >");
  }
  
};

extern "C" void init(Planet& planet)
{
  CLASS(Print, "Print any value received in bang inlet.")
  INLET(Print, bang, AnyValue, "Received values are printed out to listening notifiers.")
}