#ifndef _NEW_VALUE_H_
#define _NEW_VALUE_H_
#include "rubyk_types.h"
#include "thash.h"

union Value
{
  Value(real_t pR) : r(pR) {}
  Value(int    pI) : r((real_t)pI) {}
  Value(char * pS) : s(pS) {}
  
  char * s;
  real_t r;
  real_t f; // alias for r
  real_t i; // alias for r
};

typedef Value* Values;

struct TypedValue
{
  TypedValue() : type('\0'), value(0.0) {}
  TypedValue(char pType, Value& pVal) : type(pType), value(pVal) {}
  char  type;
  Value value;
};

typedef THash<std::string,TypedValue> Hash;
typedef std::list<std::string>::const_iterator Hash_iterator;

/*
// how about reference counting ?

val->f ===> float
val->mat => matrix

// reference counting implemented in the matrix:

Matrix * mat = val->mat;
acquire(mat);
...
release(mat); // deleted if reference count reaches 0
*/
#endif // _NEW_VALUE_H_