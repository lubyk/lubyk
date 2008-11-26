#ifndef _RUBYK_TYPES_H_
#define _RUBYK_TYPES_H_


// type for real numbers
#ifndef real_t
#define real_t double
#endif

/** Value types. */
enum value_t {
  NilValue  = 0,
  BangValue = 1,
  AnonymousValue,
  NumberValue,
  MatrixValue,
  CharMatrixValue,
};



#endif // _RUBYK_TYPES_H_