#include "class.h"
#include <iostream>

class Test : public Node
{
public:
  
  bool set (const Params& p)
  {
    mMessage = p.val("test", "Hello World !", true);
    if (mMessage == "is output ok?")
      *mOutput << "Output set" << std::endl;
      
    mCounter = p.val("counter", 0);
    mName    = p.val("name"   , std::string("no-name"));
    return true;
  }
  
  static void hello(std::ostream * pOutput, const Params& p)
  {
    *pOutput << "Hello World!\n";
  }
  
  void spy() 
  { bprint(mSpy, mSpySize,"%s: %i", mName.c_str(), mCounter); }
  
  void help()
  { *mOutput << "Don't hit me!\n"; }

  // inlet 1
  void bang(const Signal& sig)
  {
    sig.get(&mCounter);
    send(++mCounter);
    
    *mOutput << "sending nil=>";
    send(gNilSignal, 2);
    *mOutput << "<=done.\n";
  }
  
  void info(const Params& p)
  {
    *mOutput << p << std::endl;
  }
  
  
private:
  /* data */
  std::string mMessage;
  int mCounter;
  std::string mName;
};


extern "C" void init()
{ 
  CLASS (Test)
  OUTLET(Test, increment_counter)
  OUTLET(Test, send_nil)
  METHOD(Test, info)
  CLASS_METHOD(Test, hello)
}
