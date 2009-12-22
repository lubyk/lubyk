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

#include "node.h"
#include "inlet.h"
#include "outlet.h"

size_t Node::sIdCounter(0);

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