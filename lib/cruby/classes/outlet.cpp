#include "outlet.h"
#include "inlet.h"
#include "node.h"

void Outlet::compute_and_send ()
{
  Signal sig;
  
  (*mFunction)(mNode, sig);
  
  if (sig.type) send(sig); // do not send NilSignals
}

// FIXME: inline ?
void Outlet::send(const Signal& sig)
{  
  int i=1;
  LinkedList<Inlet*> * iterator = (LinkedList<Inlet*> *)(mConnections.begin());
  
  // Be careful when casting the void* to Node (slot_test uses an int as node value)
  // printf("node: %s, outlet: %i, connections: %i\n", ((Node*)mNode)->inspect(), mId, mConnections.size());
  while(iterator) {
    iterator->obj->receive(sig);
    iterator = iterator->next;
  }
}