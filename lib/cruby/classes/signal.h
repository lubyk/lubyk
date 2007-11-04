#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#include <cstdlib> // free


/** Signal types. */
enum signal_t {
  Bang = 0, /**< Trigger update without changing values. */
  Integer,  /**< Integer value. */
  Float,    /**< Float (actually double). */
  FloatArray, /**< Array of floats. Use the 'size' attribute to avoid buffer overflow. */
  VoidPointer, /**< Void pointer. If you want a malloc allocated buffer to be freed with the signal, set 'free_me' attribute to true.*/
};

typedef struct {
  signal_t    type;
  unsigned int value;
} Integer_t;

typedef struct {
  signal_t    type;
  double value;
} Float_t;

typedef struct {
  signal_t    type;
  double * value;
  size_t  size;
} FloatArray_t;

/** Element pointed by ptr gets freed with the Signal if free_me is true. */
typedef struct {
  signal_t    type;
  void * value;
  bool   free_me;
} VoidPointer_t;

/** A signal is what is transmitted along the connections. It contains a signature field along with a void value. */
union Signal {
  
  ~Signal()
  {
    if (type == VoidPointer && ptr.free_me) {
      free(ptr.value);
    }
  }
  /// set methods ///
  
  /** Set as bang. */
  inline void set_bang() { type = Bang; }
  
  /** Set as integer. */
  inline void set(int pInt)
  {
    type = Integer;
    i.value = pInt;
  }
  
  /** Set as double. */
  inline void set(double pDouble)
  {
    type = Float;
    f.value = pDouble;
  }
  
  /** Set as float. */
  inline void set(float pFloat)
  { set((double)pFloat); }
  
  /** Set as void *. */
  inline void set(void * pPtr, bool pFree)
  {
    type = VoidPointer;
    ptr.value = pPtr;
    ptr.free_me = pFree;
  }
  
  /** Set as void*. */
  inline void set(void * pPtr)
  { set(pPtr, false); }
  
  /// get methods ///
  
  /** Get as bang. */
  inline bool is_bang() const { return type == Bang; }
  
  /** Get as int. */
  inline bool get(int * pInt) const
  {
    switch(type) {
      case Integer:
        *pInt = i.value;
        return true;
      case Float:
        *pInt = (int)f.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as float. */
  inline bool get(float * pFloat) const
  {
    switch(type) {
      case Integer:
        *pFloat = (float)i.value;
        return true;
      case Float:
        *pFloat = (float)f.value;
        return true;
      default:
        return false;
    }
  }
  
  /** Get as double. */
  inline bool get(double * pFloat) const
  { 
    switch(type) {
      case Integer:
        *pFloat = (double)i.value;
        return true;
      case Float:
        *pFloat = f.value;
        return true;
      default:
        return false;
    }
  }
  
  /** get as void* */
  inline bool get(void ** pPtr) const
  { 
    switch(type) {
      case VoidPointer:
        *pPtr = ptr.value;
        return true;
      default:
        return false;
    }
  }
  
/* data */
  signal_t      type;
  Integer_t     i;
  Float_t       f;
  FloatArray_t  floats;
  VoidPointer_t ptr;
};

#endif // _SIGNAL_H_