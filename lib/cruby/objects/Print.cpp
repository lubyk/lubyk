#include "class.h"

class Print : public Node
{
public:
  
  bool init (const Params& p)
  {
    return true;
  }
  
  void bang()
  {
    // do nothing
  }
  
  // inlet 1
  void print(const Signal& sig)
  { 
    *mOutput << sig << std::endl;
  }
};

extern "C" void init()
{
  CLASS(Print)
  INLET(Print,print)
}