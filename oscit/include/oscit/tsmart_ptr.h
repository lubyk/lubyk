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

#ifndef OSCIT_INCLUDE_OSCIT_TSMART_PTR_H_
#define OSCIT_INCLUDE_OSCIT_TSMART_PTR_H_
#include <stdlib.h> // size_t

/** The TSmartPtr class implements a simple Smart Pointer with reference counting. */
template <class T>
class TSmartPtr
{
public:
  TSmartPtr (T * p = 0) : ptr_(0) {
    if (p) ptr_ = new Ptr(p);
  }
  
  /** Initialize with another TSmartPtr: light copy of content. */
  TSmartPtr(const TSmartPtr &other) {
    acquire(other.ptr_);
  }
  
  /** Light copy of the data contained in the other TSmartPtr. */
  TSmartPtr& operator=(const TSmartPtr &other) {
    if (this != &other) {
      release();
      acquire(other.ptr_);
    }
    return *this;
  }
  
  /** When the TSmartPtr dies, it releases the content (decrement refcount). */
  virtual ~TSmartPtr() {
    release();
  }
  
  T * data() {
    return (ptr_ ? ptr_->data_ : NULL);
  }
  
  /** Return reference count for the content. Only used for testing/debugging. */
  size_t ref_count() const {
    return ptr_ ? ptr_->ref_count_ : 0;
  }
  
  void acquire(const TSmartPtr& s) {
    acquire(s.ptr_);
  }
protected:
  
  /** Structure shared by all smart pointers pointing to the same data. Holds the pointer to the "content" and the reference count. */
  struct Ptr 
  {
    Ptr(T* p = 0, size_t c = 1) : data_(p), ref_count_(c) {}
    T * data_;
    size_t ref_count_;
  };
  
  Ptr * ptr_;
  
private:
  
  /** Get hold of a pointer (the TSmartPtr is in charge of calling 'delete'). */
  void acquire(Ptr * p) {  
    ptr_ = p;
    if (ptr_) {
      ptr_->ref_count_++;
    }
  }
  
  /** Decrement reference counter, destroying content if the reference count drops to zero. */
  void release() {
    if (ptr_ && --ptr_->ref_count_ == 0) {
      delete ptr_->data_;
      delete ptr_;
    }
    // stop pointing to counter+data
    ptr_ = NULL;
  }

};

#endif // OSCIT_INCLUDE_OSCIT_TSMART_PTR_H_
