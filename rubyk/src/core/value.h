#ifndef _VALUE_H_
#define _VALUE_H_
/** Value types. */
enum value_t {
  NilValue  = 0,
  BangValue = 1,
  AnonymousValue,
  NumberValue,
  MatrixValue,
};

#include "smart_ptr.h"
#include "data.h"


/** Generic wrapper for all values passed between objects through outlets and to/from methods called through osc or the command line.

This class has some knowledge on its real self and content through the virtual methods "data_type" (content-type) and "value_type" (wrapper-type). For all subclasses of +Value+, the +data_type+ can be either +NilValue+ or the same as the +value_type+.

For the wrapper class +Value+, the +value_type+ is always +AnonymousValue+ and the +data_type+ can be any valid data type. 


To implement in sub-classes:


virtual value_t value_type() const
{ return SuperValue; }

// FIXME: is it possible to use a template for these 3 methods ?

SuperValue(const Value& pOther)
{ pOther->set(this); }

const SuperData * data () const
{
  return mPtr ? (SuperData*)(mPtr->mDataPtr) : NULL;
}

SuperData * mutable_data ()
{
  if (!mPtr)
    return NULL;

  if (mPtr->mRefCount > 1)
    copy();

  return (SuperData*)(mPtr->mDataPtr);
}
*/
class Value : public SmartPtr<Data>
{
public:
  Value()
  {}
  
  Value(const Value& pOther)
  { pOther.set(this); }
  
  /** Anonymization of the content to the ancestor class +Data+. */
  Value(Data * p) : SmartPtr<Data>(p) 
  {}
  
  /** Returns the data-type (type of the content pointed by the smart pointer). */
  value_t data_type() const
  { return mPtr ? mPtr->mDataPtr->type() : NilValue; }
  
  /** Returns the value-type (type of the pointer itself). */
  virtual value_t value_type() const
  { return AnonymousValue; }
  
  /** Textual representation of the value-type. */
  const char* value_type_name() const
  { return name_from_type(value_type()); }
  
  /** Textual representation of the data-type. */
  const char* data_type_name() const
  { return name_from_type(data_type()); }
  
  /** Type conversion ("this" sets "pOther").
  
  If you wonder why we haven't implemented it the other way around like: "pOther.from(this)", the reason is that we wanted a
  consistant interface for Values and native types. You cannot write "double d; d.from(this)" so we use "set". For the same reason,
  we use a pointer instead of a reference (clearly show what is updated). */
  bool set (Value * pOther) const
  {
    switch (data_type())
    {
      // what do I contain ?
      case NumberValue:
        switch (pOther->value_type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case NumberValue:
            *pOther = *this;   // pOther acquires our content
            return true;
          default:
            return false;
        }
      default:
        return false;
    }
  }
  
  /** Set a double from the content. Return false on failure. 
  
  FIXME: we should use a template here, but it seems to break on compilation... */
  bool set (double * pResult) const
  {
    if (!mPtr) return false;
    return mPtr->mDataPtr->set(pResult);
  }
  
private:
  
  /** Return the textual representation of a value from a value type. */
  static const char* name_from_type(value_t pType)
  {  
    switch (pType)
    {
      case NilValue:
      return "Nil   ";
      case BangValue:
      return "Bang  ";
      case NumberValue:
      return "Number";
      case AnonymousValue:
      return "Anonymous";
      default:
      return "???";
    }
  }
};

/** Macro to ease Value specialization. */
#define VALUE_METHODS(klass,data_type,signature) \
  klass() {} \
  klass(data_type* p) : Value(p) {} \
  virtual ~klass() {} \
  klass(const Value& pOther) \
  { pOther.set(this); } \
  const data_type * data () const \
  { return mPtr ? data_pointer() : NULL; } \
  data_type * mutable_data () \
  { if (!mPtr) return NULL; \
    copy_if_shared(); \
    return (data_type*)(mPtr->mDataPtr); } \
  virtual value_t type() const \
  { return signature; } \
protected: \
  inline data_type * data_pointer() const \
  { return (data_type*)(mPtr->mDataPtr); } \
public: \

#endif // _VALUE_H_