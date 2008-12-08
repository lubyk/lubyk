#include "rubyk.h"

class Print : public Node
{
public:
  
  bool set (const Value& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Value& val)
  { 
    *mOutput << sig << std::endl;
  }
  
  virtual const Value inspect(const Value& val) 
  { 
    bprint(mSpy, mSpySize,"");
  }
  
};

extern "C" void init()
{
  CLASS(Print)
}