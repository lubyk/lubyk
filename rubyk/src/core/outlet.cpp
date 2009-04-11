#include "outlet.h"
#include "inlet.h"
#include "node.h"


// FIXME: inline ?
void Outlet::send(const Value &val)
{  
  LinkedList<Inlet*> * iterator = (LinkedList<Inlet*> *)(mConnections.begin());
  
  while(iterator) {
    iterator->obj->receive(val);
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