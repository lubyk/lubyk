#include "node.h"
#include "signal.h"

class Print : public Node
{
public:
  
  bool init (const Params& p)
  {
    make_inlet<Print,&Print::print>();
    return true;
  }
  
  void print(const Signal& sig)
  { inspectSignal(stdout, sig); }
};

extern "C" void init()
{
  Node::declare<Print>("Print");
}