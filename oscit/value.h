#ifndef _VALUE_H_
#define _VALUE_H_
#include "oscit/conf.h"

union Value
{  
  Value() : r(0) {}
  explicit Value(real_t pR) : r(pR) {}
  explicit Value(int    pI) : r((real_t)pI) {}
  
  real_t r;
  real_t f; // alias for r
  real_t d; // alias for r
};

/*

explicit Value(char * pS) : s(pS) {}
explicit Value(Value * pValues) : values(pValues) {}
const char * s;
Value * values; // multi-value

#include "rubyk_types.h"
#include "thash.h"
struct TypedValue
{
  TypedValue() : type('\0'), value(0.0) {}
  TypedValue(char pType, Value& pVal) : type(pType), value(pVal) {}
  char  type;
  Value value;
};

typedef THash<std::string,TypedValue> Hash;
typedef std::list<std::string>::const_iterator Hash_iterator;


// how about reference counting ?

val->f ===> float
val->mat => matrix

// reference counting implemented in the matrix:

Matrix * mat = val->mat;
acquire(mat);
...
release(mat); // deleted if reference count reaches 0
*/
#endif // _VALUE_H_