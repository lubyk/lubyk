#include "Outlet.h"
#include "Inlet.h"
#include <cstdio>

void Outlet::compute_and_send ()
{
  float value;
  LinkedList<Inlet> * iterator = (LinkedList<Inlet> *)(mConnections.begin());
  
  value = (*mFunction)(mNode);
  
  while(iterator) {
    iterator->obj->receive(value);
    iterator = iterator->next;
  }
}
