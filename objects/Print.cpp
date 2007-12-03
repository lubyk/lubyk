#include "class.h"

class Print : public Node
{
public:
  
  bool set (const Params& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    *mOutput << sig << std::endl;
  }
};

extern "C" void init()
{
  CLASS(Print)
}