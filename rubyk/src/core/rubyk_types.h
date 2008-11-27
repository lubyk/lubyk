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
  AnonymousValue = 2,
  NumberValue = 4,
  MatrixValue = 8,
  StringValue = 16,
  CharMatrixValue = 32,
  CommandValue = 64,      // contains a string with the method name "tempo" and another value as parameter like Number(123)
};                        // when the command reaches the "command" inlet (on the left side) => build url "/nodes/foo/tempo" 
                          //                                                                 => Object::trigger(url, param)


#endif // _RUBYK_TYPES_H_