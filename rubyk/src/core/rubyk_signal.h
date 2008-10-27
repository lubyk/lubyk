#ifndef _RUBYK_SIGNAL_H_
#define _RUBYK_SIGNAL_H_
#include <cstdlib> // free
#include <iostream>
#include <string>
#include "midi_message.h"
#include "matrix.h"
#include "hash.h"

class Params;
union Signal;

#define SIG_DEFAULT_META_TABLE_SIZE 20

/** Signal types. */
enum rubyk_signal_t {
  NilSignal  = 0,    /**< Do not send. */
  BangSignal = 1,    /**< Trigger update without changing values. */
  IntegerSignal,     /**< IntegerSignal value. */
  DoubleSignal,      /**< DoubleSignal (actually double). */
  MatrixSignal,      /**< Pointer to a matrix of doubles. */
  StringSignal,      /**< Pointer to a std::string. */
  MidiSignal,        /**< Pointer to a midi message. */
  VoidPointerSignal, /**< Void pointer. If you want a malloc allocated buffer to be freed with the signal, set 'free_me' attribute to true.*/
};

typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta; /**< Meta values. Values are signals defined by key->value pairs. Use sig.meta(HASH("name")) to get element "name". */
  int value;
} IntegerSignal_t;

typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  double value;
} DoubleSignal_t;

typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  const Matrix * value;
} MatrixSignal_t;

/** StringValue contains a std::string (makes a copy). */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  std::string * value;
  bool   free_me;
} StringSignal_t;

/** Message pointed by value gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  MidiMessage * value;
  bool   free_me;
} MidiSignal_t;

/** Element pointed by calue gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  void * value;
  bool   free_me;
} VoidPointerSignal_t;

/** A signal is what is transmitted along the connections. It contains a signature field along with a void value. */
union Signal {
  Signal() : type(NilSignal)
  { i.meta = NULL; }
  
  Signal(rubyk_signal_t pSigType) : type(pSigType)
  { 
    i.meta = NULL;
  }
  
  Signal(const MidiMessage& pMsg)
  {
    MidiMessage * msg = new MidiMessage(pMsg);
    type = MidiSignal;
    midi_ptr.value   = msg;
    midi_ptr.free_me = false;
    i.meta           = NULL;
  }
  
  ~Signal()
  {
    clear();
    if (i.meta) delete i.meta;
  }
  
  
  /// memory management ///
  void clear()
  {
    if (type == VoidPointerSignal && ptr.free_me) {
      free(ptr.value);
    } else if (type == StringSignal) {
      delete str_ptr.value;
    } else if (type == MidiSignal && midi_ptr.free_me) {
      delete midi_ptr.value;
    }
  }
  
  inline void clear_free_me() const // we allow this on const objects because we get hold of memory pointer
  {
    Signal * s = const_cast<Signal *>(this);
    if (s->type == MidiSignal) s->midi_ptr.free_me = false;
    else if (s->type == VoidPointerSignal) s->ptr.free_me = false;
  }
  
  /** Get a meta value. */
  template<class T>
  inline bool get_meta(T* value_ptr, uint pId) const
  {
    if (!i.meta) return false;
    Signal sig;
    if (i.meta->get(&sig, pId)) {
      return sig.get(value_ptr); // TODO: avoid temporary signal
    }
    return false;
  }
  
  /** Define a meta value. */
  template<class T>
  void set_meta(uint pId, const T& value)
  {
    if (!i.meta) {
      i.meta = new Hash<uint, Signal>(SIG_DEFAULT_META_TABLE_SIZE);
    }
    
    Signal sig;
    sig.set(value);
    i.meta->set(pId, sig); // TODO: could we optimize here to avoid temporary and copy ?
  }
  
  /// set methods ///
  
  /** Set as bang. */
  inline void set_bang() { clear(); type = BangSignal; }
  
  /** Set as nil. */
  inline void set_nil() { clear(); type = NilSignal; }
  
  /** Set as boolean. */
  inline void set(bool pBool)
  {
    clear();
    type = IntegerSignal;
    i.value = pBool ? 1 : 0;
  }
  
  /** Set as integer. */
  inline void set(int pInt)
  {
    clear();
    type = IntegerSignal;
    i.value = pInt;
  }
  
  /** Set as size_t. */
  inline void set(size_t pInt)
  {
    clear();
    type = IntegerSignal;
    i.value = pInt;
  }
  
  /** Set as unsigned integer. */
  inline void set(unsigned int pInt)
  {
    clear();
    type = IntegerSignal;
    i.value = pInt;
  }
  
  /** Set as unsigned long. */
  inline void set(long pInt)
  {
    clear();
    type = IntegerSignal;
    i.value = (int)pInt;
  }
  
  /** Set as double. */
  inline void set(double pDouble)
  {
    clear();
    type = DoubleSignal;
    d.value = pDouble;
  }
  
  /** Set as float. */
  inline void set(float pDouble)
  { set((double)pDouble); }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr, bool pFree)
  {
    clear();
    type = MidiSignal;
    midi_ptr.value = pPtr;
    midi_ptr.free_me = pFree;
  }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr)
  { set(pPtr, false); }
  
  
  /** Set as Matrix. */
  inline void set(const Matrix& mat)
  {
    clear();
    type = MatrixSignal;
    matrix.value = &mat;
  }
  
  /** Set as String. */
  inline void set(const std::string& str)
  {
    clear();
    type = StringSignal;
    str_ptr.value = new std::string(str);
  }
  
  /** Set as const char. */
  inline void set(const char * pStr)
  { set(std::string(pStr)); }
  
  /** Set from params. 
    * @param p parameters from command line.
    * @param pBuffer used to write list values from p.
    */
  void set (const Params& p, Matrix& pBuffer);
  
  /** Set as void *. */
  inline void set(void * pPtr, bool pFree)
  {
    clear();
    type = VoidPointerSignal;
    ptr.value = pPtr;
    ptr.free_me = pFree;
  }
  
  /** Set as void*. */
  inline void set(void * pPtr)
  { set(pPtr, false); }
  
  /// get methods ///
  
  /** Get as bang. */
  inline bool is_bang() const { return type == BangSignal; }
  
  /** Get as boolean. */
  inline bool get(bool * pBool) const
  {
    switch(type) {
      case IntegerSignal:
        *pBool = (i.value != 0);
        return true;
      case DoubleSignal:
        *pBool = (d.value != 0);
        return true;
      default:
        return false;
    }
  }
  
  /** Get as int. */
  inline bool get(int * pInt) const
  {
    switch(type) {
      case IntegerSignal:
        *pInt = i.value;
        return true;
      case DoubleSignal:
        *pInt = (int)d.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as size_t. */
  inline bool get(size_t * pInt) const
  {
    switch(type) {
      case IntegerSignal:
        *pInt = (size_t)i.value;
        return true;
      case DoubleSignal:
        *pInt = (size_t)d.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as long. */
  inline bool get(long * pInt) const
  {
    switch(type) {
      case IntegerSignal:
        *pInt = (long)i.value;
        return true;
      case DoubleSignal:
        *pInt = (long)d.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as float. */
  inline bool get(float * pDouble) const
  {
    switch(type) {
      case IntegerSignal:
        *pDouble = (float)i.value;
        return true;
      case DoubleSignal:
        *pDouble = (float)d.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as double. */
  inline bool get(double * pDouble) const
  { 
    switch(type) {
      case IntegerSignal:
        *pDouble = (double)i.value;
        return true;
      case DoubleSignal:
        *pDouble = d.value;
        return true;
      default:
        return false;
    }
  }
  
  /** get as matrix* */
  inline bool get(const Matrix ** pMat) const
  { 
    if (type == MatrixSignal) {
      *pMat = matrix.value;
      return true;
    } else
      return false;
  }
  
  /** get as string* */
  inline bool get(std::string * pStr) const
  { 
    if (type == StringSignal) {
      *pStr = *(str_ptr.value);
      return true;
    } else
      return false;
  }
  
  /** get as void* */
  inline bool get(void ** pPtr) const
  { 
    switch(type) {
      case VoidPointerSignal:
        *pPtr = ptr.value;
        return true;
      default:
        return false;
    }
  }
  
  
  /** Return the type as const char. */
  const char * type_name () const
  {
    switch(type) {
    case BangSignal:
      return "Bang";
    case IntegerSignal:
      return "Integer";
    case DoubleSignal:
      return "Double";
    case MatrixSignal:
      return "Matrix";
    case StringSignal:
      return "String";
    case VoidPointerSignal:
      return "VoidPointer";
    case MidiSignal:
      return "Midi";
    case NilSignal:
      return "Nil";    
    default:
      return "???";
    }
  }
  
  
/* data */
  rubyk_signal_t       type;
  IntegerSignal_t      i;
  DoubleSignal_t       d;
  MatrixSignal_t       matrix;
  StringSignal_t       str_ptr;
  MidiSignal_t         midi_ptr;
  VoidPointerSignal_t  ptr;
};

extern Signal gNilSignal;  // defined in rubyk.cpp
extern Signal gBangSignal; // defined in rubyk.cpp

std::ostream& operator<< (std::ostream& pStream, const Signal& sig);

#endif // _RUBYK_SIGNAL_H_