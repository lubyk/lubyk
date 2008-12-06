#include "slot.h"
#include "node.h"
#include "root.h"

Slot::~Slot()
{
  // remove connections with other slots
  Slot * s;
  while( mConnections.get(&s)) {
    s->remove_connection(this);
    mConnections.pop();
  }
}


void Slot::setId(int pId)
{
  mId = pId;
}

bool Slot::connect(Slot * pOther)
{  
  if (pOther == NULL) return false;
  // outlet --> inlet
  if (add_connection(pOther)) { 
    pOther->add_connection(this);
    return true;
  }
  return false;
}

void Slot::disconnect(Slot * pOther)
{
  remove_connection(pOther);
  pOther->remove_connection(this);
}
    

/** Sort slots by rightmost node and rightmost position in the same node. */
bool Slot::operator>= (const Slot& pOther) const
{ 
  if (mNode == pOther.mNode) {
    // same node, sort by position in container, largest first
    return mId < pOther.mId;
  } else {
    // different node, sort by node position, greatest first
    return ((Node*)(mNode))->trigger_position() < ((Node*)(pOther.mNode))->trigger_position();
  }
}


bool Slot::add_connection(Slot * pOther)
{ 
  if (mType & pOther->mType) {
    // only create a link if the slot type signature are compatible
    // OrderedList makes sure the link is not created again if it already exists.
    mConnections.push(pOther); 
    return true;
  } else {
    return false;
  }
}

void Slot::remove_connection(Slot * pOther)
{
  mConnections.remove(pOther);
}

const Value Slot::link(const Value& val)
{
  if (val.is_nil()) {
    // return list of links
    LinkedList<Slot*> * iterator = mConnections.begin();
    std::string res = "";
    while(iterator) {
      if (res != "") res.append(",");
      res.append(iterator->obj->url());
      iterator = iterator->next;
    }
    return String(res);
  } else {
    // create a new link
    // TODO: make sure it does not already exist.
    Object * target = mRoot->find(String(val));
    if (!target) return Error("Could not create link (").append(val.to_string()).append(" not found).");

    // FIXME: make sure it is an Inlet if this = Outlet. Type checking !!
    if (connect((Slot*)target))
      return String(val);
    else
      return Error("Could not make the connection with (").append(val.to_string()).append(").");
  }
}