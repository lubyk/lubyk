#include "class.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    mAdd     = p.get("add", 1);
    
    make_inlet<Counter,&Counter::set_add>();
    make_inlet<Counter,&Counter::set_counter>();
    make_outlet<Counter,&Counter::value>();
    
    return true;
  }

  virtual void spy() 
  { spy_print("%i", mCounter );  }
  
  virtual void help()
  { *mOutput << "Increments by 'value' each time it receives a bang.\n"; }
  
  void get_add_value(const Params& pParam)
  {
    *mOutput << "Add is " << mAdd << std::endl;
  }
  
  static void hello(std::ostream * pOutput, const Params& pParam)
  {
    *pOutput << "Hello\n";
  }
  
  void set_add(const Signal& sig)
  { SET_INTEGER(mAdd, sig);     }
  
  void set_counter(const Signal& sig)
  { SET_INTEGER(mCounter, sig); }
  

  void value(Signal& sig)
  { SEND_INTEGER(sig, mCounter += mAdd); }
  
private:
  int mCounter;
  int mAdd;
};

extern "C" void init()
{
  Class * klass = Class::declare<Counter>("Counter");
  
  klass->add_method<Counter,&Counter::get_add_value>("add");
  klass->add_class_method("hello", &Counter::hello);
}