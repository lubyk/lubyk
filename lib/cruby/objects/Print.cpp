#include "class.h"

class Print : public Node
{
public:
  
  bool init (const Params& p)
  {
    make_inlet<Print,&Print::print>();
    return true;
  }
  
  void print(const Signal& sig)
  { inspectSignal(stdout, sig); printf("\n"); }
};

extern "C" void init()
{
  Class::declare<Print>("Print");
}