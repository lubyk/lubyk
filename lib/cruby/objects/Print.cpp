#include "class.h"

class Print : public Node
{
public:
  
  bool init (const Params& p)
  {
    make_inlet<Print,&Print::print>();
    return true;
  }
  
  // inlet 1
  void print(const Signal& sig)
  { 
    *mOutput << sig << std::endl;
  }
private:
  char mBuffer[50];
};

extern "C" void init()
{
  Class::declare<Print>("Print");
}