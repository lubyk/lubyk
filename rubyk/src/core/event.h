#ifndef _EVENT_H_
#define _EVENT_H_
#include "object.h"
#include <ostream>

class Node;
class Worker;

/** This is the base class for Events and CallEvents. The class has a field for the time at which the event should
  * be triggered. */
class Event
{
 public:
  Event (void *node, time_t when) : when_(when), receiver_(node), forced_(false) {}
  
  Event () : forced_(false) {}

  virtual ~Event() {}
  
  void trigger() {
    (*function_)(receiver_,parameter_);
  }

  inline bool operator>= (const Event &event) const {
    return when_ >= event.when_;
  }
  
  inline bool uses_receiver(const void * node) const {
    return receiver_ == node;
  }
  
  void * receiver() {
    return receiver_;
  }
  
protected:
  friend class Worker;
  friend class Node;
  
  // access needed by root
  time_t when_;
  void * receiver_;
  bool   forced_;     /**< Run even if trying to quit. */
  Value  parameter_;
  
  void (*function_)(void *receiver, const Value &parameter);
};

/** This is an event that sends a bang to a node. */
class BangEvent : public Event
{
public:
  BangEvent (time_t when, Node * receiver)
  {
    when_      = when;
    receiver_  = (void*)receiver;
    function_  = &cast_bang_method;
  }
  
private:
  /** Make pointer to the bang method. */
  static void cast_bang_method (void *receiver, const Value &parameter);
};

template<class T, void(T::*Tmethod)(void*)>
class TEvent : public Event
{
public:
  TEvent (time_t when, T *receiver, const Value &parameter)
  {
    when_      = when;
    receiver_  = (void*)receiver;
    parameter_ = parameter;
    function_  = &cast_method;
  }
private:
  /** Make pointer to method for events. */
  static void cast_method (void *receiver, const Value &parameter)
  {
    (((T*)receiver)->*Tmethod)(parameter);
  }
};

#endif