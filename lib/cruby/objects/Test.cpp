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
  { spy_print("%s: %i", mName.c_str(), mCounter); }
  
  void help()
  { *mOutput << "Don't hit me!\n"; }
  
  void set_counter(const Signal& sig)
  { sig.get(&mCounter); }

  void increment_counter(Signal& sig)
  { sig.set(++mCounter); }
  
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
  Class * c = Class::declare<Test>("Test");
  c->add_class_method("hello", &Test::hello);
  c->add_inlet< Test, &Test::set_counter>("set_counter");
  c->add_outlet<Test, &Test::increment_counter>("increment");
  c->add_method<Test, &Test::info>("info");
}
