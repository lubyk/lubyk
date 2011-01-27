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

#ifndef LUBYK_INCLUDE_LUBYK_EVENT_H_
#define LUBYK_INCLUDE_LUBYK_EVENT_H_

#include "lubyk/oscit.h"

#include <ostream>

namespace rk {

class Node;
class Worker;

/** This is the base class for Events and CallEvents. The class has a field for the time at which the event should
  * be triggered. */
class Event
{
 public:
  Event (Node *node, time_t when) : when_(when), node_(node), forced_(false), parameter_(NIL_VALUE) {}
  
  Event () : forced_(false) {}

  virtual ~Event() {}
  
  void trigger() {
    (*function_)(node_,parameter_);
  }

  inline bool operator>= (const Event &event) const {
    return when_ >= event.when_;
  }
  
  inline bool uses_receiver(const void * node) const {
    return node_ == node;
  }
  
  const Node *node() const { return node_; }
  
  void set_forced(bool forced) {
    forced_ = forced;
  }
  
protected:
  friend class Worker; // TODO: remove these
  friend class Node;   // TODO: remove these
  
  time_t when_;
  Node * node_;
  bool   forced_;     /**< Run even if trying to quit. */
  Value  parameter_;
  
  void (*function_)(Node *receiver, const Value &parameter);
};

/** This is an event that sends a bang to a node. */
class BangEvent : public Event
{
public:
  BangEvent(Node *receiver, time_t when, bool forced = false) {
    when_      = when;
    node_  = receiver;
    function_  = &cast_bang_method;
    forced_    = forced;
  }
  
  BangEvent(Node *receiver, time_t when, const Value &parameter, bool forced = false) {
    when_      = when;
    node_  = receiver;
    function_  = &cast_bang_method;
    parameter_ = parameter;
    forced_    = forced;
  }
  
private:
  /** Make pointer to the bang method. */
  static void cast_bang_method(Node *receiver, const Value &parameter);
};

template<class T, void(T::*Tmethod)(const Value&)>
class TEvent : public Event
{
public:
  TEvent (time_t when, T *node, const Value &parameter)
  {
    when_      = when;
    node_      = node;
    parameter_ = parameter;
    function_  = &cast_method;
  }
private:
  /** Make pointer to method for events. */
  static void cast_method(Node *node, const Value &parameter) {
    (((T*)node)->*Tmethod)(parameter);
  }
};

} // rk

#endif // LUBYK_INCLUDE_LUBYK_EVENT_H_
