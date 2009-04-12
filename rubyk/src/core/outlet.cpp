#include "outlet.h"
#include "inlet.h"
#include "node.h"


// FIXME: inline ?
void Outlet::send(const Value &val)
{  
  LinkedList<Inlet*> * iterator = (LinkedList<Inlet*> *)(connections_.begin());
  
  while(iterator) {
    iterator->obj->receive(val);
    iterator = iterator->next;
  }
}

void Outlet::register_in_node()
{
  node_->register_outlet(this);
}

void Outlet::unregister_in_node()
{
  node_->unregister_outlet(this);
}