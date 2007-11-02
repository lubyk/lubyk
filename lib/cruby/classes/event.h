#ifndef _EVENT_H_
#define _EVENT_H_

class BaseEvent
{
public:
  virtual ~BaseEvent() {}
  
  void trigger()
  { mParameter ? (*mFunction)(mReceiver,mParameter) : (*mVoidFunction)(mReceiver); }

  inline bool operator>= (const BaseEvent& pOther) const
  { return mTime >= pOther.mTime; }
  
  long double mTime;
  
protected:
  void * mParameter;
  void * mReceiver;
  void (*mVoidFunction)(void * pReceiver);
  void (*mFunction)(void * pReceiver, void * pParam);
};


template<class T, void(T::*Tmethod)(void)>
class CallEvent : public BaseEvent
{
public:
  CallEvent (long double pTime, T * pReceiver) {
    mTime      = pTime;
    mReceiver  = (void*)pReceiver;
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
  Event (long double pTime, T * pReceiver, void * pParam) {
    mTime      = pTime;
    mReceiver  = (void*)pReceiver;
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