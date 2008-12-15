#ifndef _EVENT_H_
#define _EVENT_H_
#include "object.h"
#include <ostream>

/** Pointer to a method. */
typedef void (*obj_method_t)(void * pReceiver, const Value& val);

/** This is the base class for Events and CallEvents. The class has a field for the time at which the event should
  * be triggered. */
class Event
{
public:
  Event (time_t pTime, void * pNode, bool pIsBang = true) : mTime(pTime), mReceiver(pNode), mForced(false) {}
  
  Event () : mForced(false) {}

  virtual ~Event() {}
  
  void trigger()
  { mHasParameter ? (*mFunction)(mReceiver,mParameter) : (*mVoidFunction)(mReceiver); }

  inline bool operator>= (const Event& pOther) const
  { return mTime >= pOther.mTime; }
  
  bool uses_receiver(const void * pNode) const
  { return pNode == mReceiver; }
  
  const void * node() const
  { return mReceiver; }
  
protected:
  friend class Root;
  friend class Node;
  
  // access needed by root
  time_t mTime;
  void * mReceiver;
  bool mForced; /**< Run even if trying to quit. */
  bool mHasParameter;
  
  void * mParameter;
  void (*mVoidFunction)(void * pReceiver);
  void (*mFunction)(void * pReceiver, void * pParam);
};

/** This is an event for void method. */
template<class T, void(T::*Tmethod)(void)>
class TCallEvent : public Event
{
public:
  TCallEvent (time_t pTime, T * pReceiver)
  {
    mTime      = pTime;
    mReceiver  = (void*)pReceiver;
    mHasParameter = false;
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
class TEvent : public Event
{
public:
  TEvent (time_t pTime, T * pReceiver, void * pParam)
  {
    mTime      = pTime;
    mReceiver  = (void*)pReceiver;
    mParameter = pParam;
    mHasParameter = true;
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