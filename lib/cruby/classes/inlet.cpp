#include "inlet.h"
#include "node.h"

void Inlet::receive (const Signal& sig)
{
  // set value using callback
  (*mFunction)(mNode, sig);
  
  // trigger a bang if this is the first inlet
  if (mId == 0) ((Node*)mNode)->bang();
}
