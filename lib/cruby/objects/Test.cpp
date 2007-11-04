#include "class.h"

class Test : public Node
{
public:
  bool init (const Params& p)
  {
    mMessage = p.get("test", "Hello World !");
    return true;
  }
  
  static void hello(std::ostream * pOutput, const Params& pParam)
  {
    *pOutput << "Hello World!\n";
  }
private:
  /* data */
  std::string mMessage;
};



extern "C" void init()
{ 
  Class * klass = Class::declare<Test>("Test");
  klass->add_class_method("hello", &Test::hello);
}
