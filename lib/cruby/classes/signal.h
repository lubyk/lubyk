#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#include <stdlib.h> // free

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
typedef union {
  signal_t      type;
  Integer_t     i;
  Float_t       f;
  FloatArray_t  floats;
  VoidPointer_t ptr;
} Signal;

/** Called this before the signal is destroyed. */
inline void clearSignal(Signal& sig)
{
  if (sig.type == VoidPointer && sig.ptr.free_me) {
    free(sig.ptr.value);
  }
}

/** Prints some information on the signal content. */
inline void inspectSignal(FILE * pStream, const Signal& sig)
{
  switch(sig.type) {
    case Bang:
      fprintf(pStream, "<Bang>");
      break;
    case Integer:
      fprintf(pStream, "%i",sig.i.value);
      break;
    case Float:
      fprintf(pStream, "%.2f",sig.f.value);
      break;
    case FloatArray:
      fprintf(pStream, "[%i,%i]",sig.floats.value, sig.floats.size);
      break;
    case VoidPointer:
      fprintf(pStream, "[%p,%i]",sig.ptr.value, sig.ptr.free_me);
      break;
    default:
      fprintf(pStream, "??");
  }
}

/** MACROS TO SET/GET VALUES */
#define SET_INTEGER(lval, sig) {if (sig.type == Integer) lval = sig.i.value; \
  else if (sig.type == Float) lval = (int)sig.f.value;}

#define SET_FLOAT(lval, sig) {if (sig.type == Float) lval = sig.f.value; \
  else if (sig.type == Integer) lval = (float)sig.i.value;}

#define SEND_INTEGER(sig, rval) { sig.type = Integer; sig.i.value = (rval); }
#define SEND_FLOAT(sig, rval) { sig.type = Float; sig.f.value = (rval); }
#define SEND_BANG(sig) { sig.type = Bang; }

#endif // _SIGNAL_H_