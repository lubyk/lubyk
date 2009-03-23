#ifndef _VALUE_H_
#define _VALUE_H_
#include "oscit/conf.h"

/** Value is a union which can contain real_t values or pointers to other types of values.

This type is very simple for efficiency reasons and so that it can be easily shared with 'C' as a POD.
There is no 'append', 'type' or 'size' or anything that would require some knowledge of the actual content of
the union. If you need these functionalities, it should not be too hard to write a TypedValue wrapper. */
union Value
{ 
  Value() : r(0) {}
  explicit Value(real_t pR) : r(pR) {}
  explicit Value(int    pI) : r((real_t)pI) {}
  explicit Value(Value * pValues) : list(pValues) {}
  
  /** Allocate space for a list of values. */
  static Value * alloc(size_t pSize)
  {
    Value * list = (Value*)malloc(pSize * sizeof(Value));
    if (list == NULL)
      return NULL;
    
    for (size_t i = 0; i < pSize; i++)
      list[i].f = 0.0;
    return list;
  }
  
  /** Free memory allocated for list. */
  void dealloc()
  {
    free(list);
  }
  
  real_t r;
  real_t f; // alias for r
  real_t d; // alias for r
  Value * list; // multi-value
};

/*

explicit Value(char * pS) : s(pS) {}
const char * s;

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