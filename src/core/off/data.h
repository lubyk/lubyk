/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"
#include <sstream>

/** Base class for all data transmitted along objects or used as parameter/return for methods. */
class Data
{
public:
  Data() 
  {
#ifdef _TESTING_
  mId = ++sIdCounter;
#endif
  }
  
  virtual ~Data() {}
  
  virtual Data * clone()
  {
    return new Data(*this);
  }
  
  virtual void copy(Data * pOther)
  {
    // do nothing
  }
  
  virtual value_t type() const
  { return BangValue; }
  
  const char * type_name() const
  { return name_from_type(type()); }
  
  virtual bool set (real_t& pResult)
  { return false; }
  
  /** String representation of data into stream. */
  virtual void to_stream(std::ostream& pStream) const
  {
    pStream << type_name();
  }
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    pStream << type_name();
  }
  
  /** JSON representation of data. */
  std::string to_json() const
  {
    std::ostringstream os(std::ostringstream::out);
    to_json(os);
    return os.str();
  }
  
  /** String representation of data. */
  std::string to_json() const
  {
    std::ostringstream os(std::ostringstream::out);
    to_stream(os);
    return os.str();
  }
  
  /** Return the textual representation of a value from a value type. */
  static const char* name_from_type(value_t pType)
  {  
    switch (pType)
    {
      case NilValue:
      return "Nil";
      case BangValue:
      return "Bang!";
      case AnonymousValue:
      return "Anonymous";
      case NumberValue:
      return "Number";
      case MatrixValue:
      return "Matrix";
      case StringValue:
      return "String";
      case CharMatrixValue:
      return "CharMatrix";
      case CommandValue:
      return "Command";
      case HashValue:
      return "Hash";
      case ErrorValue:
      return "Error";
      default:
      return "???";
    }
  }
  
  /** Explicit conversion to a 'real_t'. Returns the default value if not implemented in the specialized Data. */
  virtual Real convert(const real_t& pDefault) const
  {
    printf("Data (real_t) version in [%s]\n", type_name());
    return pDefault;
  }
  
  /** Explicit conversion to a 'real_t'. Returns the default value if not implemented in the specialized Data. */
  virtual int convert(const int& pDefault) const
  {
    printf("Data (int) version.\n");
    return pDefault;
  }
  
  /** Explicit conversion to a 'std::string'. Returns the default value if not implemented in the specialized Data. */
  virtual std::string convert(const std::string &pDefault) const
  {
    return pDefault;
  }
  
#ifdef _TESTING_
 size_t id() const
 { return mId; }

 static size_t sIdCounter;
 static bool   sShowId;
 size_t        mId;
#endif
};

/** Macro to ease Data specialization. */
#define DATA_METHODS(klass,signature) \
  klass() {} \
  virtual Data * clone() \
  { return new klass(*this); } \
  virtual value_t type() const \
  { return signature; } \


#endif // _DATA_H_