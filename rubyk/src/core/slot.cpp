#include "slot.h"
#include "node.h"
#include "planet.h"

Slot::~Slot() {
  // remove connections with other slots
  Slot * s;
  while( connections_.get(&s)) {
    s->remove_connection(this);
    connections_.pop();
  }
}


bool Slot::connect(Slot *slot) {  
  if (slot == NULL) return false;
  // outlet --> inlet
  if (add_connection(slot)) {
    // two way connection
    slot->add_connection(this);
    return true;
  }
  return false;
}

void Slot::disconnect(Slot *slot) {
  remove_connection(slot);
  slot->remove_connection(this);
}
    

/** Sort slots by rightmost node and rightmost position in the same node. */
bool Slot::operator>= (const Slot &slot) const { 
  if (node_ == slot.node_) {
    // same node, sort by position in container, largest first
    return id_ < slot.id_;
  } else {
    // different node, sort by node position, greatest first
    return ((Node*)(node_))->trigger_position() < ((Node*)(slot.node_))->trigger_position();
  }
}


bool Slot::add_connection(Slot *slot) { 
  if (type_ == slot->type_) {
    // only create a link if the slot type signature are the same
    // OrderedList makes sure the link is not created again if it already exists.
    connections_.push(slot); 
    return true;
  } else {
    return false;
  }
}

void Slot::remove_connection(Slot * slot) {
  connections_.remove(slot);
}

const Value Slot::change_link(unsigned char operation, const Value &val) {
  if (val.is_string()) {
    // update a link (create/destroy)
    
    BaseObject * target = root_->object_at(val.s);
    if (!target) return ErrorValue(NOT_FOUND_ERROR, val.s);
    
    if (target->class_type() == H("Object")) {
      target = target->first_child();
      if (!target) {
        return ErrorValue(NOT_FOUND_ERROR, val.s).append(": slot not found");
      }
    }
    
    if (class_type() == H("Outlet")) {
      // other should be an Inlet
      target = (Slot*)TYPE_CAST(Inlet,  &target);
    } else {
      // other should be an Outlet
      target = (Slot*)TYPE_CAST(Outlet, &target);
    }
    
    if (!target)
      return ErrorValue(BAD_REQUEST_ERROR, "Could not update link with '").append(val.to_string()).append("': incompatible).");
    
    if (pCreate){
      // connect
      if (connect((Slot*)target)) {
        //std::cout << "LINKED: " << url() << " with " << val << std::endl;
        return String(val);
      } else
        return Error("Could not make the connection with (").append(val.to_string()).append(").");
    } else {
      // disconnect
      disconnect((Slot*)target);
      return String(val);
    }
  }
}
