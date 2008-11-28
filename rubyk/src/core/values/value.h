#ifndef _VALUE_H_
#define _VALUE_H_
#include "rubyk_types.h"
#include "smart_ptr.h"
#include "data.h"

class Error;
/** Generic wrapper for all values passed between objects through outlets and to/from methods called through osc or the command line.

This class has some knowledge on its real self and content through the virtual methods "data_type" (content-type) and "type" (wrapper-type). For all subclasses of +Value+, the +data_type+ can be either +NilValue+ or the same as the +type+.

For the wrapper class +Value+, the +type+ is always +AnonymousValue+ and the +data_type+ can be any valid data type. 
*/
class Value : public SmartPtr<Data>
{
public:
  Value()
  {}
  
  Value(const Value& pOther)
  { pOther.set(*this); }
  
  /** Anonymization of the content to the ancestor class +Data+. */
  Value(Data * p) : SmartPtr<Data>(p) 
  {}
  
  /** Returns the data-type (type of the content pointed by the smart pointer). */
  value_t data_type() const
  { return mPtr ? mPtr->mDataPtr->type() : NilValue; }
  
  /** Returns the value-type (type of the pointer itself). */
  virtual value_t type() const
  { return AnonymousValue; }
  
  
  /** Return true if the object's data is an ErrorValue. */
  bool error() const
  { return data_type() == ErrorValue; }
  
  /** Return true if the object's data is a NilValue. */
  bool nil() const
  { return !mPtr; }
  
  /** Textual representation of the value-type. */
  const char* type_name() const
  { return name_from_type(type()); }
  
  /** Textual representation of the data-type. */
  const char* data_type_name() const
  { return name_from_type(data_type()); }
  
  /** Type conversion ("this" sets "pOther").
  
  If you wonder why we haven't implemented it the other way around like: "pOther.from(this)", the reason is that we wanted a
  consistant interface for Values and native types. You cannot write "real_t d; d.from(this)" so we use "set". For the same reason,
  we use a pointer instead of a reference (clearly show what is updated). */
  bool set (Value& pOther) const
  {
    switch (data_type())
    {
      // what do I contain ?
      case NumberValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case NumberValue:
            pOther = *this;   // pOther acquires our content
            return true;
          default:
            return false;
        }
        
      case MatrixValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case MatrixValue:
            pOther = *this;   // pOther acquires our content
            return true;
          case NumberValue:     // first value in matrix ?
          case CharMatrixValue: // cast ?
          default:
            return false;
        }
        
      case StringValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case StringValue:
            pOther = *this;   // pOther acquires our content
            return true;
          default:
            return false;
        }
        
      case CharMatrixValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case CharMatrixValue:
            pOther = *this;   // pOther acquires our content
            return true;
          case NumberValue:    // first value in matrix ?  
          case MatrixValue:    // cast ?
          default:
            return false;
        }
        
      case ErrorValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case ErrorValue:
            pOther = *this;   // pOther acquires our content
            return true;
          default:
            return false;
        }
      
      default:
        return false;
    }
  }
  
  /** Set a real_t from the content. Return false on failure. 
  
  FIXME: we should use a template here, but it seems to break on compilation... */
  bool set (real_t& pResult) const
  {
    if (!mPtr) return false;
    return mPtr->mDataPtr->set(pResult);
  }
  
#ifdef _TESTING_
  size_t data_id()
  {
    if (mPtr) {
      return mPtr->mDataPtr->mId;
    } else {
      return 0;
    }
  }
#endif
  
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
      case AnonymousValue:
      return "Anonymous";
      case NumberValue:
      return "Number";
      case MatrixValue:
      return "Matrix";
      case StringValue:
      return "String";
      case CharMatrixValue:
      return "CharMatrix";
      case CommandValue:
      return "Command";
      case ErrorValue:
      return "Error";
      default:
      return "???";
    }
  }
};

/** Macro to ease Value specialization. 
   *mutable_data* never returns NULL. New data is created on demand.
   */
#define VALUE_METHODS(klass,data_type,signature,super) \
  klass() {} \
  klass(data_type * d) : super(d) {} \
  virtual ~klass() {} \
  klass(const Value& pOther) \
  { pOther.set(*this); } \
  const data_type * data () const \
  { return mPtr ? data_pointer() : NULL; } \
  const data_type * operator->() const \
  { return data(); } \
  const data_type& operator*() const \
  { return *data(); } \
  data_type * mutable_data () \
  { if (!mPtr) make_data_ptr(); \
    copy_if_shared(); \
    return (data_type*)(mPtr->mDataPtr); } \
  virtual value_t type() const \
  { return signature; } \
protected: \
  inline void make_data_ptr () \
  { mPtr = new Ptr(new data_type()); } \
  inline data_type * data_pointer() const \
  { return (data_type*)(mPtr->mDataPtr); } \
public: \

#endif // _VALUE_H_