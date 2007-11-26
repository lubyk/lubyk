#ifndef _RUBYK_SIGNAL_H_
#define _RUBYK_SIGNAL_H_
#include <cstdlib> // free
#include <iostream>
#include "midi_message.h"
#include "buf.h"


/** Signal types. */
enum rubyk_signal_t {
  NilSignal  = 0,    /**< Do not send. */
  BangSignal = 1,    /**< Trigger update without changing values. */
  IntegerSignal,     /**< IntegerSignal value. */
  CharSignal,        /**< IntegerSignal value. */
  DoubleSignal,       /**< DoubleSignal (actually double). */
  ArraySignal,  /**< Array of floats. Use the 'size' attribute to avoid buffer overflow. */
  MidiSignal,        /**< Pointer to a midi message. */
  VoidPointerSignal, /**< Void pointer. If you want a malloc allocated buffer to be freed with the signal, set 'free_me' attribute to true.*/
};

typedef struct {
  rubyk_signal_t    type;
  int value;
} IntegerSignal_t;

typedef struct {
  rubyk_signal_t    type;
  char value;
} CharSignal_t;

typedef struct {
  rubyk_signal_t    type;
  double value;
} DoubleSignal_t;

typedef struct {
  rubyk_signal_t    type;
  double * value;
  size_t  size;
} ArraySignal_t;

/** Message pointed by value gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  MidiMessage * value;
  bool   free_me;
} MidiSignal_t;

/** Element pointed by calue gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  void * value;
  bool   free_me;
} VoidPointerSignal_t;

/** A signal is what is transmitted along the connections. It contains a signature field along with a void value. */
union Signal {
  Signal() {}
  Signal(rubyk_signal_t pSigType) : type(pSigType) {}
  Signal(const MidiMessage& pMsg)
  {
    MidiMessage * msg = new MidiMessage(pMsg);
    type = MidiSignal;
    midi_ptr.value = msg;
    midi_ptr.free_me = true;
  }
  
  ~Signal()
  {
    if (type == VoidPointerSignal && ptr.free_me) {
      free(ptr.value);
    } else if (type == MidiSignal && midi_ptr.free_me) {
      delete midi_ptr.value;
    }
  }
  /// memory management ///
  inline void clear_free_me() const // we allow this on const objects because we get hold of memory pointer
  {
    Signal * s = const_cast<Signal *>(this);
    if (s->type == MidiSignal) s->midi_ptr.free_me = false;
    else if (s->type == VoidPointerSignal) s->ptr.free_me = false;
  }
  
  /// set methods ///
  
  /** Set as bang. */
  inline void set_bang() { type = BangSignal; }
  
  /** Set as nil. */
  inline void set_nil() { type = NilSignal; }
  
  
  /** Set as integer. */
  inline void set(int pInt)
  {
    type = IntegerSignal;
    i.value = pInt;
  }
  
  /** Set as char. */
  inline void set(char pChar)
  {
    type = CharSignal;
    c.value = pChar;
  }
  
  /** Set as unsigned integer. */
  inline void set(unsigned int pInt)
  {
    type = IntegerSignal;
    i.value = pInt;
  }
  
  /** Set as unsigned long. */
  inline void set(long pInt)
  {
    type = IntegerSignal;
    i.value = (int)pInt;
  }
  
  /** Set as double. */
  inline void set(double pDouble)
  {
    type = DoubleSignal;
    d.value = pDouble;
  }
  
  /** Set as float. */
  inline void set(float pDouble)
  { set((double)pDouble); }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr, bool pFree)
  {
    type = MidiSignal;
    midi_ptr.value = pPtr;
    midi_ptr.free_me = pFree;
  }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr)
  { set(pPtr, false); }
  
  
  /** Set as double *. */
  inline void set(double * pPtr, size_t pSize)
  {
    type = ArraySignal;
    array.value = pPtr;
    array.size  = pSize;
  }
  
  /** Set as double* from Buf<double>. */
  inline void set(Buf<double>& pBuf)
  {
    type = ArraySignal;
    array.value = pBuf.data;
    array.size  = pBuf.size;
  }
  
  /** Set as void *. */
  inline void set(void * pPtr, bool pFree)
  {
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

  /** Get as char. */
  inline bool get(char * pChar) const
  {
    switch(type) {
      case IntegerSignal:
        *pChar = (char)i.value;
        return true;
      case CharSignal:
        *pChar = c.value;
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
  
  /** get as double* */
  inline bool get(double ** pArray) const
  { 
    if (type == ArraySignal) {
      *pArray = array.value;
      return true;
    } else
      return false;
  }
  
  
  /** Return the type as const char. */
  const char * type_name () const
  {
    switch(type) {
    case BangSignal:
      return "BangSignal";
    case IntegerSignal:
      return "IntegerSignal";
    case DoubleSignal:
      return "DoubleSignal";
    case ArraySignal:
      return "ArraySignal";
    case VoidPointerSignal:
      return "VoidPointerSignal";
    case MidiSignal:
      return "MidiSignal";
    case NilSignal:
      return "NilSignal";    
    default:
      return "???";
    }
  }
  
  
/* data */
  rubyk_signal_t       type;
  IntegerSignal_t      i;
  CharSignal_t         c;
  DoubleSignal_t       d;
  ArraySignal_t  array;
  MidiSignal_t         midi_ptr;
  VoidPointerSignal_t  ptr;
};

extern Signal gNilSignal;  // defined in rubyk.cpp
extern Signal gBangSignal; // defined in rubyk.cpp

// inline for convenience (avoid another build object)
inline std::ostream& operator<< (std::ostream& pStream, const Signal& sig)
{
  char buffer[50];
  switch(sig.type) {
  case BangSignal:
    pStream << "Bang!";
    break;
  case IntegerSignal:
    pStream << sig.i.value;
    break;
  case DoubleSignal:
    snprintf(buffer, 50, "%.2f", sig.d.value);
    pStream << buffer;
    break;
  case ArraySignal:
    if (sig.array.size == 0) {
      snprintf(buffer, 50, "<%s %p,%i>",sig.type_name(), sig.array.value, sig.array.size);
      pStream << buffer;
    } else {
      int sz = 16;
      int start;
      if (sz > sig.array.size) sz = sig.array.size;
      start = sig.array.size - sz;
      snprintf(buffer, 50, "<%s %p (% .2f", sig.type_name(), sig.array.value, sig.array.value[start]);
      pStream << buffer;
      for (int i= start+1; i < start+sz; i++) {
        snprintf(buffer, 50, ", % .2f", sig.array.value[i]);
        pStream << buffer;
      }
      pStream << ")," << sig.array.size << ">";
    }
    break;
  case VoidPointerSignal:
    snprintf(buffer, 50, "<%s %p,%i>",sig.type_name(), sig.ptr.value, sig.ptr.free_me);
    pStream << buffer;
    break;
  case MidiSignal:
    pStream << "<" << sig.type_name() << " " << *(sig.midi_ptr.value) << ">";
    break;
  default:
    pStream << "<" << sig.type_name() << ">";
  }
  return pStream;
}


template<>
inline bool Buf<double>::set(const Signal& sig)
{
  if(sig.type != ArraySignal) return false;
  return set(sig.array.value, sig.array.size);
}
#endif // _RUBYK_SIGNAL_H_