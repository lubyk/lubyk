#ifndef _VALUE_TEMPLATE_H
#define _VALUE_TEMPLATE_H
#include "value.h"

/** Template to ease Value specialization. */
template<value_t V, class T> // T = Data type
class ValueTemplate : public Value
{
public:
  ValueTemplate(T* p) : Value(p) {}

  ValueTemplate() {}
  
  ValueTemplate(const Value& pOther)
  { pOther.set(this); }

  /** This method needs to be specialized for each template instanciation. */
  virtual value_t value_type() const
  { return V; }

  /** Return a const pointer to the data in the SmartPointer. */
  const T * data () const
  {
    return mPtr ? data_pointer() : NULL;
  }

  /** Return a pointer to mutable data contained in the SmartPointer. Makes a copy if needed. */
  T * mutable_data ()
  {
    if (!mPtr)
      return NULL;

    if (mPtr->mRefCount > 1)
      copy();

    return (T*)(mPtr->mDataPtr);
  }
protected:
  inline T * data_pointer() const
  {
    return (T*)(mPtr->mDataPtr);
  }
};

#endif // _VALUE_TEMPLATE_H