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

#ifndef OSCIT_INCLUDE_OSCIT_TYPED_H_
#define OSCIT_INCLUDE_OSCIT_TYPED_H_
#include <stdio.h> // NULL

// Using RTTI does not seem to work with libs and such (rubyk does not recognize oscit objects...)
// #define USE_RTTI_TYPE

#include <iostream>

namespace oscit {

#define TYPED(str) \
  virtual const char *class_path() const { return str; } \
  static  const char *_path() { return str; } \

#ifdef USE_RTTI_TYPE

#define kind_of(klass) is_a<klass *>()

class Typed {
public:
  TYPED("")
  virtual ~Typed() {}

  template<class T>
  inline bool is_a() {
    return dynamic_cast<T>(this) != NULL;
  }

  template<class T>
  inline T type_cast() {
    return dynamic_cast<T>(this);
  }

  const std::string class_name() const {
    std::string path(class_path());
    size_t pos = path.find_last_of('.');
    return pos == std::string::npos ? path : path.substr(pos+1);
  }
};

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) dynamic_cast<klass *>(op)

#else

#define kind_of(klass) is_a(klass::_path())

class Typed
{
public:
  TYPED("")
  virtual ~Typed() {}

  template<class T>
  static inline T * type_cast(Typed * obj) {
    if (!obj) return NULL;
    if (obj->is_a(T::_path())) {
      return (T*)obj;
    } else {
      return NULL;
    }
  }

  bool is_a(const char *path) {
    const char * c  = path;
    const char * cl = class_path();
    while (*c) {
      if (*c != *cl) return false;
      ++c;
      ++cl;
    }
    return (*cl == '\0') || (*cl == '.');
  }

  const std::string class_name() const {
    std::string path(class_path());
    size_t pos = path.find_last_of('.');
    return pos == std::string::npos ? path : path.substr(pos+1);
  }

};

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) oscit::Typed::type_cast<klass>(op)

#endif  // USE_RTTI_TYPE

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_TYPED_H_
