#include "Inlet.h"
#include "Node.h"

void Inlet::receive (float value)
{
  // set value using callback
  (*mFunction)(mNode, value);
  
  // trigger a bang if this is the first inlet
  if (mId == 0) ((Node*)mNode)->bang();
  
}
