/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "rubyk/inlet.h"

#include "rubyk/node.h"
#include "rubyk/outlet.h"

namespace rk {

Inlet::~Inlet() {
  unregister_in_node();

  std::list<Outlet*>::iterator it = connected_outlets_.begin();
  std::list<Outlet*>::iterator end = connected_outlets_.end();

  while(it != end) {
    (*it)->remove_connection(this);
    ++it;
  }
}

void Inlet::register_in_node() {
  node_->register_inlet(this);
}

void Inlet::unregister_in_node() {
  node_->unregister_inlet(this);
}

bool Inlet::operator>=(const Inlet &other) const {
  if (node_ == other.node_) {
    // same node, sort by position in container, largest first
    return id_ < other.id_;
  } else {
    // different node, sort by node position, greatest first
    return node_->trigger_position() < other.node_->trigger_position();
  }
}

// Called by Outlet once type compatibility is checked.
bool Inlet::add_connection(Outlet *inlet) {
  std::list<Outlet*>::iterator it  = connected_outlets_.begin();
  std::list<Outlet*>::iterator end = connected_outlets_.end();

  while(it != end) {
    // make sure it is not already in list
    if (*it == inlet) return true;
    ++it;
  }

  connected_outlets_.push_back(inlet);
  return true;
}

void Inlet::remove_connection(Outlet *outlet) {
  connected_outlets_.remove(outlet);
}


void Inlet::sort_incoming_connections() {
  std::list<Outlet*>::iterator it  = connected_outlets_.begin();
  std::list<Outlet*>::iterator end = connected_outlets_.end();

  while(it != end) {
    (*it)->sort_connections();
    ++it;
  }
}

} // rk
