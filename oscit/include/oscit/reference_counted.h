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

#ifndef OSCIT_INCLUDE_REFERENCE_COUNTED_H_
#define OSCIT_INCLUDE_REFERENCE_COUNTED_H_

namespace oscit {

class ReferenceCounted {
 public:
  ReferenceCounted() : ref_count_(1) {}
  
  virtual ~ReferenceCounted() {}
  
  size_t ref_count() { return ref_count_; }
  
  template<class T>
  static T* acquire(T *elem) {
    ++elem->ref_count_;
    return elem;
  }
  
  template<class T>
  static T* release(T *elem) {
    if (elem) {
      if (--elem->ref_count_ == 0) delete elem;
    }
    return NULL;
  }
 protected:
  size_t ref_count_;
};

} // oscit

#endif // OSCIT_INCLUDE_REFERENCE_COUNTED_H_
