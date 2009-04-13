#include "inlet.h"
#include "outlet.h"
#include "node.h"

void Inlet::register_in_node() {
  node_->register_inlet(this);
}

void Inlet::unregister_in_node() {
  node_->unregister_inlet(this);
}

void Inlet::sort_incoming_connections() {
  LinkedList<Outlet*> * iterator = (LinkedList<Outlet*> *)(connections_.begin());

  while(iterator) {
    iterator->obj->sort_connections((Slot*)this);
    iterator = iterator->next;
  }
}