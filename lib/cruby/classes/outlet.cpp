#include "outlet.h"
#include "inlet.h"

void Outlet::compute_and_send ()
{
  Signal sig;
  
  (*mFunction)(mNode, sig);
  send(sig);
}

// FIXME: inline ?
void Outlet::send(const Signal& sig)
{  
  LinkedList<Inlet> * iterator = (LinkedList<Inlet> *)(mConnections.begin());
  while(iterator) {
    iterator->obj->receive(sig);
    iterator = iterator->next;
  }
}