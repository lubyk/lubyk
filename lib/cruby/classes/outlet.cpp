#include "outlet.h"
#include "inlet.h"

void Outlet::compute_and_send ()
{
  Signal sig;
  LinkedList<Inlet> * iterator = (LinkedList<Inlet> *)(mConnections.begin());
  
  (*mFunction)(mNode, sig);
  
  while(iterator) {
    iterator->obj->receive(sig);
    iterator = iterator->next;
  }
}
