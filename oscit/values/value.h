#ifndef _VALUE_H_
#define _VALUE_H_
#include "rubyk_types.h"
#include "smart_ptr.h"
#include "data.h"
#include <ostream>

class Error;
/** Generic wrapper for all values passed between objects through outlets and to/from methods called through osc or the command line.

This class has some knowledge on its real self and content through the virtual methods "data_type" (content-type) and "type" (wrapper-type). For all subclasses of +Value+, the +data_type+ can be either +NilValue+ or the same as the +type+.

For the wrapper class +Value+, the +type+ is always +AnonymousValue+ and the +data_type+ can be any valid data type. 
*/
class Value : public SmartPtr<Data>
{
public:
  Value() {}
  
  Value(const std::string& s)
  {
    from_string(s);
  }
  
  Value(const char * s)
  {
    from_string(std::string(s));
  }
  
  Value(const Value& pOther)
  { pOther.set(*this); }
  
  /** Anonymization of the content to the ancestor class +Data+. */
  Value(Data * p) : SmartPtr<Data>(p) {}
  
  /** Parse string to build a value from a json representation. */
  size_t from_string(const std::string& s)
  { return from_string(s.c_str()); }
  
  /** Parse string to build a value from a json representation. */
  size_t from_string(const char * s);
  
  /** Returns the value-type (type of the pointer itself). */
  virtual value_t type() const
  { return AnonymousValue; }
  
  /** Returns the data-type (type of the content pointed by the smart pointer). */
  value_t data_type() const
  { return mPtr ? mPtr->mDataPtr->type() : NilValue; }
  
  /** Return true if the object's data is Nil. */
  inline bool is_nil() const
  { return !mPtr; }
  
  /** Return true if the object's data is a BangData. */
  inline bool is_bang() const
  { return data_type() == BangValue; }
  
  /** Return true if the object's data is a NumberData. */
  inline bool is_number() const
  { return data_type() == NumberValue; }
  
  /** Return true if the object's data is a MatrixData. */
  inline bool is_matrix() const
  { return data_type() == MatrixValue; }
  
  /** Return true if the object's data is a StringData. */
  inline bool is_string() const
  { return data_type() == StringValue; }
  
  /** Return true if the object's data is a CharMatrixData. */
  inline bool is_char_matrix() const
  { return data_type() == CharMatrixValue; }
  
  /** Return true if the object's data is a CommandData. */
  inline bool is_command() const
  { return data_type() == CommandValue; }
  
  /** Return true if the object's data is a HashData. */
  inline bool is_hash() const
  { return data_type() == HashValue; }
  
  /** Return true if the object's data is an ErrorData. */
  inline bool is_error() const
  { return data_type() == ErrorValue; }
  
  
  /** Textual representation of the value-type. */
  const char* type_name() const
  { return Data::name_from_type(type()); }
  
  /** Textual representation of the data-type. */
  const char* data_type_name() const
  { return Data::name_from_type(data_type()); }
  
  /** Type conversion ("this" sets "pOther").
  
  If you wonder why we haven't implemented it the other way around like: "pOther.from(this)", the reason is that we wanted a
  consistant interface for Values and native types. You cannot write "real_t d; d.from(this)" so we use "set". */
  bool set (Value& pOther) const
  {
    switch (data_type())
    {
      // what do I contain ?
      case BangValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case BangValue:
            pOther = *this;   // pOther acquires our content
            return true;
          default:
            return false;
        }

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
        
      case HashValue:
        switch (pOther.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case HashValue:
            pOther = *this;   // pOther acquires our content
            return true;
          case StringValue:    // convert to hash ?
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
  
  inline Data * data_pointer() const
  { 
    return mPtr->mDataPtr; 
  }
  
  /** Set a real_t from the content. Return false on failure. 
  
  FIXME: we should use a template here, but it seems to break on compilation... */
  bool set (real_t& pResult) const
  {
    if (!mPtr) return false;
    return mPtr->mDataPtr->set(pResult);
  }
  
  /** Try to convert the data to 'U', returning default on failure. */
  template<typename U>
  const U operator|| (const U& pDefault) const
  {
    return is_nil() ? pDefault : mPtr->mDataPtr->convert(pDefault);
  }
  
  std::string to_string() const
  {
    return mPtr ? mPtr->mDataPtr->to_string() : std::string("Nil");
  }
  
#ifdef _TESTING_
  size_t data_id() const
  {
    if (mPtr) {
      return mPtr->mDataPtr->mId;
    } else {
      return 0;
    }
  }
#endif
private:
  friend std::ostream& operator<< (std::ostream& os, const Value val);
};

/** Macro to ease Value specialization. 
   *mutable_data* never returns NULL. New data is created on demand.
   */
#define VALUE_METHODS(klass,data_type,signature,super)              \
  klass() {}                                                        \
  klass(data_type * d) : super(d) {}                                \
  virtual ~klass() {}                                               \
  klass(const Value& pOther)                                        \
  { pOther.set(*this); }                                            \
  const data_type * data () const                                   \
  { return mPtr ? data_pointer() : NULL; }                          \
  const data_type * operator->() const                              \
  { return data(); }                                                \
  const data_type& operator*() const                                \
  { return *data(); }                                               \
  data_type * mutable_data ()                                       \
  { if (!mPtr) make_data_ptr();                                     \
    copy_if_shared();                                               \
    return (data_type*)(mPtr->mDataPtr); }                          \
  virtual value_t type() const                                      \
  { return signature; }                                             \
protected:                                                          \
  inline void make_data_ptr ()                                      \
  { mPtr = new Ptr(new data_type()); }                              \
  inline data_type * data_pointer() const                           \
  { return (data_type*)(mPtr->mDataPtr); }                          \
public: \

#define VALUE_FROM_STRING(klass)                                    \
  klass(const std::string& s)                                       \
  { Value v(s);                                                     \
    v.set(*this); }                                                 \
  klass(const char * s)                                             \
  { Value v(s);                                                     \
    v.set(*this); }                                                 \
  klass& operator= (const std::string& s)                           \
  { Value(s).set(*this);                                            \
    return *this; }                                                 \
    
std::ostream& operator<< (std::ostream& pStream, const Value val);
#endif // _VALUE_H_