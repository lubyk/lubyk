#include "outlet.h"
#include "inlet.h"
#include "node.h"


// FIXME: inline ?
void Outlet::send(const Value& sig)
{  
  LinkedList<Inlet*> * iterator = (LinkedList<Inlet*> *)(mConnections.begin());
  
  //printf("out: %s:%i\n", ((Node*)mNode)->name(),mId);
  
  // Be careful when casting the void* to Node (slot_test uses an int as node value)
  // printf("node: %s, outlet: %i, connections: %i\n", ((Node*)mNode)->inspect(), mId, mConnections.size());
  while(iterator) {
    iterator->obj->receive(sig);
    iterator = iterator->next;
  }
}

void Outlet::register_in_node()
{
  mNode->register_outlet(this);
}

void Outlet::unregister_in_node()
{
  mNode->unregister_outlet(this);
}