#include "node.h"

class Print : public Node
{
public:
  
  bool init (const Params& p)
  {
    make_inlet<Print,&Print::print>();
    return true;
  }
  
  void print(float value)
  { printf("%.2f\n",value); }
};

extern "C" void init()
{
  Node::declare<Print>("Print");
}