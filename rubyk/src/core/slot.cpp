#include "slot.h"
#include "node.h"
#include "planet.h"

Slot::~Slot()
{
  // remove connections with other slots
  Slot * s;
  while( connections_.get(&s)) {
    s->remove_connection(this);
    connections_.pop();
  }
}


void Slot::set_id(int pId)
{
  mId = pId;
}

bool Slot::connect(Slot * slot)
{  
  if (slot == NULL) return false;
  // outlet --> inlet
  if (add_connection(slot)) { 
    slot->add_connection(this);
    return true;
  }
  return false;
}

void Slot::disconnect(Slot * slot)
{
  remove_connection(slot);
  slot->remove_connection(this);
}
    

/** Sort slots by rightmost node and rightmost position in the same node. */
bool Slot::operator>= (const Slot& slot) const
{ 
  if (node_ == slot.node_) {
    // same node, sort by position in container, largest first
    return mId < slot.mId;
  } else {
    // different node, sort by node position, greatest first
    return ((Node*)(node_))->trigger_position() < ((Node*)(slot.node_))->trigger_position();
  }
}


bool Slot::add_connection(Slot * slot)
{ 
  if (mType & slot->mType) {
    // only create a link if the slot type signature are compatible
    // OrderedList makes sure the link is not created again if it already exists.
    connections_.push(slot); 
    return true;
  } else {
    return false;
  }
}

void Slot::remove_connection(Slot * slot)
{
  connections_.remove(slot);
}

const Value Slot::change_link(const Value val, bool pCreate)
{
  if (val.is_nil()) {
    // return list of links
    LinkedList<Slot*> * iterator = connections_.begin();
    std::string res = "";
    while(iterator) {
      if (res != "") res.append(",");
      res.append(iterator->obj->url());
      iterator = iterator->next;
    }
    return String(res);
  } else {
    // update a link (create/destroy)
    
    // TODO: make sure it does not already exist.
    
    oscit::Object * target = mRoot->find(String(val));
    if (!target) return Error("Could not update link (").append(val.to_string()).append(": not found).");
    
    if (target->type() == H("oscit::Object")) {
      target = target->first_child();
      if (!target) return Error("Could not update link (").append(val.to_string()).append(": Slot not found).");
    }
    
    if (type() == H("Outlet")) {
      target = (Slot*)TYPE_CAST(Inlet,target);
    } else {
      target = (Slot*)TYPE_CAST(Outlet,target);
    }
    
    if (!target) return Error("Could not update link (").append(val.to_string()).append(": incompatible).");
    
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
