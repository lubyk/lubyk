#include <cstdio>
#include <stdlib.h>

enum signal_t {
  Bang = 0,
  Integer,
  Float,
  FloatArray,
  VoidPointer,
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

typedef union {
  signal_t      type;
  Integer_t     i;
  Float_t       f;
  FloatArray_t  floats;
  VoidPointer_t ptr;
} Signal;

inline void clearSignal(Signal& pSig)
{
  if (pSig.type == VoidPointer && pSig.ptr.free_me) {
    free(pSig.ptr.value);
  }
}

void inspectSignal(Signal& pSig)
{
  switch(pSig.type) {
    case Bang:
      printf("<Bang>");
      break;
    case Integer:
      printf("%i",pSig.i.value);
      break;
    case Float:
      printf("%.2f",pSig.f.value);
      break;
    case FloatArray:
      printf("[%i,%i]",pSig.floats.value, pSig.floats.size);
      break;
    case VoidPointer:
      printf("[%p,%i]",pSig.ptr.value, pSig.ptr.free_me);
      break;
    default:
      printf("??");
  }
}

int main()
{
  Signal sig1,sig2,sig3;
  sig1.type = Bang;
  sig2.type = Float;
  sig3.type = VoidPointer;
  sig3.ptr.free_me = true;
  
  sig3.ptr.value = malloc(900 * sizeof(double));
  sig2.f.value = 456.3245;
  
  inspectSignal(sig1);
  printf("\n");
  inspectSignal(sig2);
  printf("\n");
  
  clearSignal(sig1);
  clearSignal(sig2);
  clearSignal(sig3);
  
  return 0;
}