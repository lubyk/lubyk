#include "rubyk.h"

class Print : public Node
{
public:
  Print() : output_(&std::count) {}
  
  // [1]
  void bang(const Value &val) { 
    *output_ << val << std::endl;
  }
  
  void set_output(std::ostream &output) { output_ = &output; }
  
  std::ostream * output_;
};

extern "C" void init(Worker& planet)
{
  CLASS(Print, "Print any value received in bang inlet.", "no options")
  INLET(Print, bang, H("*"), "Received values are printed out to listening notifiers.")
}