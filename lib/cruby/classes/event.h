#ifndef _EVENT_H_
#define _EVENT_H_

#include "params.h"
#include <ostream>

class Rubyk;
class Class;
class Node;
union Signal;

/** Pointer to a function to create nodes. */
typedef Node * (*create_function_t)(Class * pClass, Rubyk * pServer, const Params& p, std::ostream * pOutput);

/** Pointer to a class method that can be called from the command line with "Value.method(Params)" */
typedef void (*class_method_t)(std::ostream * pOutput, const Params& p);

/** Pointer to an inlet method that can be called from the command line with "obj.method(Params)" */
typedef void (*inlet_method_t)(void * pReceiver, const Signal& sig);

/** Pointer to an inlet method that can be called from the command line with "obj.method(Params)" */
typedef void (*outlet_method_t)(void * pReceiver, Signal& sig);


/** This is the base class for Events and CallEvents. The class has a field for the time at which the event should
  * be triggered. */
class BaseEvent
{
public:
  virtual ~BaseEvent() {}
  
  void trigger()
  { mParameter ? (*mFunction)(mNode,mParameter) : (*mVoidFunction)(mNode); }

  inline bool operator>= (const BaseEvent& pOther) const
  { return mTime >= pOther.mTime; }
  
  time_t mTime;
  
protected:
  void * mParameter;
  void * mNode;
  void (*mVoidFunction)(void * pReceiver);
  void (*mFunction)(void * pReceiver, void * pParam);
};


template<class T, void(T::*Tmethod)(void)>
class CallEvent : public BaseEvent
{
public:
  CallEvent (time_t pTime, T * pReceiver) {
    mTime      = pTime;
    mNode  = (void*)pReceiver;
    mParameter = NULL;
    mVoidFunction = &cast_void_method;
  }
  
private:
  /** Make pointer to method for events. */
  static void cast_void_method (void * pReceiver)
  {
    (((T*)pReceiver)->*Tmethod)();
  }
  
};

template<class T, void(T::*Tmethod)(void*)>
class Event : public BaseEvent
{
public:
  Event (time_t pTime, T * pReceiver, void * pParam) {
    mTime      = pTime;
    mNode  = (void*)pReceiver;
    mParameter = pParam;
    mFunction  = &cast_method;
  }
private:
  /** Make pointer to method for events. */
  static void cast_method (void * pReceiver, void * pParam)
  {
    (((T*)pReceiver)->*Tmethod)(pParam);
  }
};

#endif