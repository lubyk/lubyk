#include "inlet.h"
#include "node.h"

void Inlet::receive (const Value &val)
{
  // std::cout << mId << "receive " << val << std::endl;
  // TODO: test speed and replace by functor if there is no real gain...
  if (mId == 0) 
    ((Node*)mNode)->bang(val);   // use virtual (faster)
  else
    (*method_)(mNode, val);    // use functor
}


void Inlet::register_in_node()
{
  mNode->register_inlet(this);
}

void Inlet::unregister_in_node()
{
  mNode->unregister_inlet(this);
}