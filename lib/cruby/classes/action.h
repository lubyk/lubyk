#ifndef _ACTION_H_
#define _ACTION_H_
#include "node.h"
#include "params.h"

#ifndef member_method_t
/** Pointer to a member method that can be called from the command line with "obj.method(Params)" */
typedef void (*member_method_t)(void * pReceiver, const Params& p);
#endif

/** Any action from a command line or editor must be registered in the commands' queue and will be triggered by the
  * server at an appropriate time. This class is a pure virtual base for the commands. All commands must register an
  * output stream for the command result. */
class Action
{
public:
  Action() : mOutput(&std::cout) {}
  Action(std::ostream * pOutput) : mOutput(pOutput) {}
  
  virtual ~Action() {}
  virtual void trigger() = 0;
protected:
  std::ostream * mOutput;
};

/** We can use vtables here since command actions occur really less often then events and speed is not an issue. */
class CallMethodAction : public Action
{
public:
  CallMethodAction(Node * pReceiver, member_method_t pMethod, const Params& pParam, std::ostream * pOutput) : mNode(pReceiver), mParams(pParam), mFunction(pMethod), Action(pOutput) {}
  
  void trigger()
  { 
    mNode->set_output(mOutput);
    (*mFunction)(mNode, mParams); 
  }
  
private:
  Node * mNode;
  Params mParams;
  member_method_t mFunction;
};

#endif