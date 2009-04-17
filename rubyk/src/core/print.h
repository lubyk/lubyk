#ifndef _PRINT_H_
#define _PRINT_H_
#include "node.h"
#include <ostream>

class Print : public Node
{
public:
  TYPED("Object.Node.Print")
  
  Print() : output_(&std::cout) {}
  
  // [1]
  void bang(const Value &val) { 
    *output_ << val << std::endl;
  }
  
  void set_output(std::ostream &output) { output_ = &output; }
  
  std::ostream * output_;
};

#endif // _PRINT_H_