#include "node.h"
#include "inlet.h"
#include "outlet.h"

Node::~Node() {
  // we have to do this here before ~Node, because some events have to be triggered before the node dies (note off).
  remove_my_events();
  unloop_me();
  
  for(std::vector<Outlet*>::iterator it = outlets_.begin(); it < outlets_.end(); it++) {
    delete *it;
  }
  
  for(std::vector<Inlet*>::iterator it = inlets_.begin(); it < inlets_.end(); it++) {
    delete *it;
  }
}

void Node::sort_connections() {
  for(std::vector<Inlet*>::iterator it = inlets_.begin(); it < inlets_.end(); it++) {
    (*it)->sort_incoming_connections();
  }
}