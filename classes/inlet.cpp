#include "inlet.h"
#include "node.h"

void Inlet::receive (const Signal& sig)
{
  //printf("in: %s:%i\n", ((Node*)mNode)->name(),mId);
  
  if (mId == 0 || sig.is_bang()) 
    ((Node*)mNode)->bang(sig);   // use virtual (faster)
  else
    (*mFunction)(mNode, sig);    // use functor
}
