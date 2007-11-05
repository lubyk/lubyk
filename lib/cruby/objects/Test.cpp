#include "class.h"

class Test : public Node
{
public:
  bool init (const Params& p)
  {
    mMessage = p.get("test", "Hello World !");
    if (mMessage == "is output ok?")
      *mOutput << "Output set" << std::endl;
      
    mCounter = p.get("counter", 0);
    mName    = p.get("name"   , std::string("no-name"));
    
    make_inlet <Test,&Test::set_counter>();
    make_outlet<Test,&Test::increment_counter>();
    
    return true;
  }
  
  static void hello(std::ostream * pOutput, const Params& pParam)
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
private:
  /* data */
  std::string mMessage;
  int mCounter;
  std::string mName;
};



extern "C" void init()
{ 
  Class * klass = Class::declare<Test>("Test");
  klass->add_class_method("hello", &Test::hello);
}
