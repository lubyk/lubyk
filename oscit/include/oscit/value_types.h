/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef OSCIT_INCLUDE_OSCIT_VALUE_TYPES_H_
#define OSCIT_INCLUDE_OSCIT_VALUE_TYPES_H_

namespace oscit {
class Value;
class Hash;

extern Value gNilValue;
extern Value gTrueValue;
extern Value gFalseValue;
extern Value gEmptyValue;
extern Hash  gEmptyHash;

enum ValueType
{
  EMPTY_VALUE  = 0,
  NIL_VALUE,
  REAL_VALUE,
  STRING_VALUE,
  ERROR_VALUE,
  LIST_VALUE,
  HASH_VALUE,
  MATRIX_VALUE,
  MIDI_VALUE,
  ANY_VALUE,
};

enum ValueTypeTag
{ 
  NIL_TYPE_TAG    = 'N',
  REAL_TYPE_TAG   = 'f',
  STRING_TYPE_TAG = 's',
  ERROR_TYPE_TAG  = 'E',
  HASH_TYPE_TAG   = 'H',
  MATRIX_TYPE_TAG = 'M',
  MIDI_MESSAGE_TYPE_TAG = 'm',
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
#define NO_TYPE_TAG_ID H("")

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_VALUE_TYPES_H_
