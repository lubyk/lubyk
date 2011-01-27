/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#include "lubyk/outlet.h"
#include "lubyk/inlet.h"
#include "lubyk/node.h"

namespace rk {

// The CorderedList is thread-safe for all operations except iteration
// where an explicit lock is required.

Outlet::~Outlet() {
  unregister_in_node();

  // remove connections with other slots
  ScopedLock lock(connected_inlets_);
  LinkedList<Inlet*> *it = connected_inlets_.begin();

  while(it) {
    it->obj->remove_connection(this);
    it = it->next;
  }
}

void Outlet::send(const Value &val) {
  ScopedLock lock(connected_inlets_);
  LinkedList<Inlet*> *it = connected_inlets_.begin();

  while(it) {
    it->obj->trigger(val);
    it = it->next;
  }
}

const Value Outlet::to_hash() {
  Value result(attributes_);

  { ScopedLock lock(connected_inlets_);
    LinkedList<Inlet*> *it = connected_inlets_.begin();
    while(it) {
      result.set(it->obj->url(), HashValue());
      it = it->next;
    }
  }

  return result;
}

void Outlet::register_in_node() {
  node_->register_outlet(this);
}

void Outlet::unregister_in_node() {
  node_->unregister_outlet(this);
}

bool Outlet::connect(Inlet *inlet) {
  if (inlet == NULL) return false;
  // outlet --> inlet

  if (add_connection(inlet)) {
    // two way connection
    inlet->add_connection(this);
    return true;
  }
  return false;
}

void Outlet::disconnect(Inlet *inlet) {
  remove_connection(inlet);
  inlet->remove_connection(this);
}

bool Outlet::add_connection(Inlet *inlet) {
  if (type_id() == NO_TYPE_TAG_ID) return false;
  if (inlet->can_receive(type_id())) {
    // inlet can receive our type
    // OrderedList makes sure the link is not created again if it already exists.
    connected_inlets_.push(inlet);
    return true;
  } else {
    return false;
  }
}

void Outlet::remove_connection(Inlet *inlet) {
  connected_inlets_.remove(inlet);
}

void Outlet::sort_connections() {
  connected_inlets_.sort();
}


const Value Outlet::set(const Value &hash) {
  Value result;
  HashIterator it;
  HashIterator end = hash.end();
  Value param;
  Value change_result;

  for (it = hash.begin(); it != end; ++it) {
    // ignore methods that start with '@'.
    if ((*it).size() > 0 && (*it).at(0) == '@') continue;

    if (hash.get(*it, &param)) {
      if (param.is_nil()) {
        // disconnect
        change_result = change_link('d', Value(*it));
        if (change_result.is_error()) {
          result.set(*it, change_result);
        } else {
          result.set(*it, gNilValue);
        }
      } else {
        // connect
        change_result = change_link('c', Value(*it));
        if (change_result.is_error()) {
          result.set(*it, change_result);
        } else {
          result.set(*it, HashValue());
        }
      }
    }
  }
  return result;
}

const Value Outlet::change_link(unsigned char operation, const Value &val) {
  if (val.is_string()) {
    // update a link (create/destroy)

    ObjectHandle target;
    if (!root_->get_object_at(val.str(), &target)) return ErrorValue(NOT_FOUND_ERROR, val.str());

    if (target->kind_of(Node)) {
      if (target->first_child(&target)) {
        if (!target->kind_of(Inlet)) {
          return ErrorValue(NOT_FOUND_ERROR, val.str()).append(": inlet not found (first child is not an Inlet).");
        }
      } else {
        return ErrorValue(NOT_FOUND_ERROR, val.str()).append(": no Inlet found.");
      }
    }

    Inlet *inlet = target.type_cast<Inlet>();

    if (!inlet) {
      return ErrorValue(BAD_REQUEST_ERROR, "Could not update link with ").append(val.to_json()).append(": incompatible).");
    }

    if (operation == 'c') {
      // create link
      if (connect(inlet)) {
        // std::cout << "LINKED: " << url() << " with " << val << std::endl;
        return Value(url()).push_back("=>").push_back(target->url());
      } else {
        return ErrorValue(BAD_REQUEST_ERROR, "Could not make the connection with (").append(val.to_json()).append(").");
      }
    } else {
      // disconnect
      disconnect(inlet);
      return Value(url()).push_back("||").push_back(target->url());
    }
  } else {
    return Value(info());
  }
}

} // rk
