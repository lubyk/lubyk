#ifndef _OSCIT_VALUE_TYPES_H_
#define _OSCIT_VALUE_TYPES_H_

namespace oscit {
class Value;
class Hash;

extern Value gNilValue;
extern Value gEmptyValue;
extern Hash  gEmptyHash;

enum ValueType
{
  EMPTY_VALUE  = 0,
  NIL_VALUE    = 1,
  REAL_VALUE   = 2,
  STRING_VALUE = 3,
  ERROR_VALUE  = 4,
  LIST_VALUE   = 5,
  HASH_VALUE   = 6,
  MATRIX_VALUE = 7,
  ANY_VALUE    = 8, // nil but with '*' type tag
};

enum ValueTypeTag
{ 
  NIL_TYPE_TAG    = 'N',
  REAL_TYPE_TAG   = 'f',
  STRING_TYPE_TAG = 's',
  ERROR_TYPE_TAG  = 's',
  HASH_TYPE_TAG   = 'H',
  MATRIX_TYPE_TAG = 'M',
  ANY_TYPE_TAG    = '*',
};

enum HashDefaultSize
{
  DEFAULT_HASH_TABLE_SIZE = 20,
};

/** Wrapper around a string identifying an osc type list. */
struct TypeTag
{
  explicit TypeTag(const char *str) : str_(str) {}
  const char *str_;
};

/** Unique identifier for osc type tags strings. */
typedef uint TypeTagID;

#define ANY_TYPE_TAG_ID H("*")

} // oscit
#endif