/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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
    return node_->trigger_position() < slot.node_->trigger_position();
  }
}


bool Slot::add_connection(Slot *slot) {
  if (type_id() == NO_TYPE_TAG_ID ||  slot->type_id() == NO_TYPE_TAG_ID) return false; // one of them is a NoIO
  assert(type().size() > 0);
  assert(slot->type().size()  > 0);
  if ((kind_of(Inlet)        && can_receive(slot->type()[0])) ||
      (slot->kind_of(Inlet)  && slot->can_receive(type()[0]))) {
    // same type signature or inlet receiving any type
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
    
    Object * target = root_->object_at(val.str());
    if (!target) return ErrorValue(NOT_FOUND_ERROR, val.str());
    
    if (!target->kind_of(Slot)) {
      target = target->child("in");
      if (target) target = target->first_child(); // target/out/... 
      if (!target) {
        return ErrorValue(NOT_FOUND_ERROR, val.str()).append(": slot not found");
      }
    }
    
    if (kind_of(Outlet)) {
      // other should be an Inlet
      target = (Slot*)TYPE_CAST(Inlet, target);
    } else {
      // other should be an Outlet
      target = (Slot*)TYPE_CAST(Outlet,target);
    }
    
    if (!target) {
      return ErrorValue(BAD_REQUEST_ERROR, "Could not update link with ").append(val.to_json()).append(": incompatible).");
    }
    
    if (operation == 'c') {
      // create link
      if (connect((Slot*)target)) {
        //std::cout << "LINKED: " << url() << " with " << val << std::endl;
        return Value(url()).push_back("=>").push_back(target->url());
      } else {
        return ErrorValue(BAD_REQUEST_ERROR, "Could not make the connection with (").append(val.to_json()).append(").");
      }
    } else {
      // disconnect
      disconnect((Slot*)target);
      return Value(url()).push_back("||").push_back(target->url());
    }
  } else {
    return Value(info());
  }
}
