#ifndef _RUBYK_SIGNAL_H_
#define _RUBYK_SIGNAL_H_
#include <cstdlib> // free
#include <iostream>
#include "midi_message.h"

/** Signal types. */
enum rubyk_signal_t {
  BangSignal = 0, /**< Trigger update without changing values. */
  IntegerSignal,  /**< IntegerSignal value. */
  FloatSignal,    /**< FloatSignal (actually double). */
  FloatArraySignal, /**< Array of floats. Use the 'size' attribute to avoid buffer overflow. */
  MidiPointerSignal, /**< Pointer to a midi message. */
  VoidPointerSignal, /**< Void pointer. If you want a malloc allocated buffer to be freed with the signal, set 'free_me' attribute to true.*/
};

typedef struct {
  rubyk_signal_t    type;
  unsigned int value;
} IntegerSignal_t;

typedef struct {
  rubyk_signal_t    type;
  double value;
} FloatSignal_t;

typedef struct {
  rubyk_signal_t    type;
  double * value;
  size_t  size;
} FloatArraySignal_t;

/** Message pointed by value gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  MidiMessage * value;
  bool   free_me;
} MidiPointerSignal_t;

/** Element pointed by calue gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  void * value;
  bool   free_me;
} VoidPointerSignal_t;

/** A signal is what is transmitted along the connections. It contains a signature field along with a void value. */
union Signal {
  
  ~Signal()
  {
    if (type == VoidPointerSignal && ptr.free_me) {
      free(ptr.value);
    } else if (type == MidiPointerSignal && midi_ptr.free_me) {
      delete midi_ptr.value;
    }
  }
  /// set methods ///
  
  /** Set as bang. */
  inline void set_bang() { type = BangSignal; }
  
  /** Set as integer. */
  inline void set(int pInt)
  {
    type = IntegerSignal;
    i.value = pInt;
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
    type = FloatSignal;
    f.value = pDouble;
  }
  
  /** Set as float. */
  inline void set(float pFloat)
  { set((double)pFloat); }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr, bool pFree)
  {
    type = MidiPointerSignal;
    midi_ptr.value = pPtr;
    midi_ptr.free_me = pFree;
  }
  
  /** Set as MidiMessage* . */
  inline void set(MidiMessage * pPtr)
  { set(pPtr, false); }
  
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
      case FloatSignal:
        *pInt = (int)f.value;
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
      case FloatSignal:
        *pInt = (long)f.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as float. */
  inline bool get(float * pFloat) const
  {
    switch(type) {
      case IntegerSignal:
        *pFloat = (float)i.value;
        return true;
      case FloatSignal:
        *pFloat = (float)f.value;
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
      case FloatSignal:
        *pDouble = f.value;
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
  
/* data */
  rubyk_signal_t      type;
  IntegerSignal_t     i;
  FloatSignal_t       f;
  FloatArraySignal_t  floats;
  MidiPointerSignal_t midi_ptr;
  VoidPointerSignal_t ptr;
};


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
  case FloatSignal:
    snprintf(buffer, 50, "%.2f", sig.f.value);
    pStream << buffer;
    break;
  case FloatArraySignal:
    snprintf(buffer, 50, "#<floats %i,%i>",sig.floats.value, sig.floats.size);
    pStream << buffer;
    break;
  case VoidPointerSignal:
    snprintf(buffer, 50, "#<ptr %p,%i>",sig.ptr.value, sig.ptr.free_me);
    pStream << buffer;
    break;
  case MidiPointerSignal:
    pStream << "#<note "<< *(sig.midi_ptr.value) << ">";
    break;
  default:
    pStream << "#<\?\?>";
  }
  return pStream;
}


#endif // _RUBYK_SIGNAL_H_