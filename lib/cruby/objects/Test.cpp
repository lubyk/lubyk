#include "class.h"
#include <iostream>

class Test : public Node
{
public:
  bool init (const Params& p)
  {
    mMessage = p.val("test", "Hello World !");
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
  
  void set_counter(const Signal& sig)
  { sig.get(&mCounter); }

  void increment_counter(Signal& sig)
  { sig.set(++mCounter); }
  
  void send_nil(Signal& sig)
  { *mOutput << "nil\n"; sig.set_nil(); }
  
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
  INLET (Test, set_counter)
  OUTLET(Test, increment_counter)
  OUTLET(Test, send_nil)
  METHOD(Test, info)
  CLASS_METHOD(Test, hello)
}
