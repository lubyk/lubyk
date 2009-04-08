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
class Value : public TSmartPtr<Data>
{
public:
  Value() {}
  
  Value(const std::string &s)
  {
    from_string(s);
  }
  
  Value(const char * s)
  {
    from_string(std::string(s));
  }
  
  Value(const Value& other)
  { other.set(*this); }
  
  /** Anonymization of the content to the ancestor class +Data+. */
  Value(Data * p) : TSmartPtr<Data>(p) {}
  
  /** Parse string to build a value from a json representation. */
  size_t from_string(const std::string &s)
  { return from_string(s.c_str()); }
  
  /** Parse string to build a value from a json representation. */
  size_t from_string(const char * s);
  
  /** Returns the value-type (type of the pointer itself). */
  virtual value_t type() const
  { return AnonymousValue; }
  
  /** Returns the data-type (type of the content pointed by the smart pointer). */
  value_t data_type() const
  { return ptr_ ? ptr_->dataPtr_->type() : NilValue; }
  
  /** Return true if the object's data is Nil. */
  inline bool is_nil() const
  { return !ptr_; }
  
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
  
  /** Type conversion ("this" sets "other").
  
  If you wonder why we haven't implemented it the other way around like: "other.from(this)", the reason is that we wanted a
  consistant interface for Values and native types. You cannot write "Real d; d.from(this)" so we use "set". */
  bool set (Value& other) const
  {
    switch (data_type())
    {
      // what do I contain ?
      case BangValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case BangValue:
            other = *this;   // other acquires our content
            return true;
          default:
            return false;
        }

      // what do I contain ?
      case NumberValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case NumberValue:
            other = *this;   // other acquires our content
            return true;
          default:
            return false;
        }
        
      case MatrixValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case MatrixValue:
            other = *this;   // other acquires our content
            return true;
          case NumberValue:     // first value in matrix ?
          case CharMatrixValue: // cast ?
          default:
            return false;
        }
        
      case StringValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case StringValue:
            other = *this;   // other acquires our content
            return true;
          default:
            return false;
        }
        
      case CharMatrixValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case CharMatrixValue:
            other = *this;   // other acquires our content
            return true;
          case NumberValue:    // first value in matrix ?  
          case MatrixValue:    // cast ?
          default:
            return false;
        }
        
      case HashValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case HashValue:
            other = *this;   // other acquires our content
            return true;
          case StringValue:    // convert to hash ?
          default:
            return false;
        }
      
      case ErrorValue:
        switch (other.type())
        {
          // what does the "other" wrapper expect ?
          case AnonymousValue: // anything
          case ErrorValue:
            other = *this;   // other acquires our content
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
    return ptr_->dataPtr_; 
  }
  
  /** Set a Real from the content. Return false on failure. 
  
  FIXME: we should use a template here, but it seems to break on compilation... */
  bool set (Real& pResult) const
  {
    if (!ptr_) return false;
    return ptr_->dataPtr_->set(pResult);
  }
  
  /** Try to convert the data to 'U', returning default on failure. */
  template<typename U>
  const U operator|| (const U& pDefault) const
  {
    return is_nil() ? pDefault : ptr_->dataPtr_->convert(pDefault);
  }
  
  std::string to_string() const
  {
    return ptr_ ? ptr_->dataPtr_->to_string() : std::string("Nil");
  }
  
#ifdef _TESTING_
  size_t data_id() const
  {
    if (ptr_) {
      return ptr_->dataPtr_->mId;
    } else {
      return 0;
    }
  }
#endif
private:
  friend std::ostream& operator<< (std::ostream& os, const Value& val);
};

/** Macro to ease Value specialization. 
   *mutable_data* never returns NULL. New data is created on demand.
   */
#define VALUE_METHODS(klass,data_type,signature,super)              \
  klass() {}                                                        \
  klass(data_type * d) : super(d) {}                                \
  virtual ~klass() {}                                               \
  klass(const Value& other)                                        \
  { other.set(*this); }                                            \
  const data_type * data () const                                   \
  { return ptr_ ? data_pointer() : NULL; }                          \
  const data_type * operator->() const                              \
  { return data(); }                                                \
  const data_type& operator*() const                                \
  { return *data(); }                                               \
  data_type * mutable_data ()                                       \
  { if (!ptr_) make_data_ptr();                                     \
    copy_if_shared();                                               \
    return (data_type*)(ptr_->dataPtr_); }                          \
  virtual value_t type() const                                      \
  { return signature; }                                             \
protected:                                                          \
  inline void make_data_ptr ()                                      \
  { ptr_ = new Ptr(new data_type()); }                              \
  inline data_type * data_pointer() const                           \
  { return (data_type*)(ptr_->dataPtr_); }                          \
public: \

#define VALUE_FROM_STRING(klass)                                    \
  klass(const std::string &s)                                       \
  { Value v(s);                                                     \
    v.set(*this); }                                                 \
  klass(const char * s)                                             \
  { Value v(s);                                                     \
    v.set(*this); }                                                 \
  klass& operator= (const std::string &s)                           \
  { Value(s).set(*this);                                            \
    return *this; }                                                 \
    
std::ostream& operator<< (std::ostream& pStream, const Value& val);
#endif // _VALUE_H_