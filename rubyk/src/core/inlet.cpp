#include "inlet.h"
#include "node.h"

void Inlet::receive (const Value& sig)
{
  if (mId == 0 || sig.is_bang()) 
    ((Node*)mNode)->bang(sig);   // use virtual (faster)
  else
    (*mMethod)(mNode, sig);    // use functor
}


void Inlet::register_in_node()
{
  mNode->register_inlet(this);
}

void Inlet::unregister_in_node()
{
  mNode->unregister_inlet(this);
}