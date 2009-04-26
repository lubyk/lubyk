#ifndef _EVENT_H_
#define _EVENT_H_
#include "oscit.h"
#include <ostream>

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

#endif