#include "inlet.h"
#include "node.h"

void Inlet::receive (const Signal& sig)
{
  //printf("in: %s:%i\n", ((Node*)mNode)->name(),mId);
  // set value using callback
  (*mFunction)(mNode, sig);
  
  // trigger a bang if this is the first inlet or we received a bang
  if (mId == 0 || sig.is_bang()) ((Node*)mNode)->bang();
}
