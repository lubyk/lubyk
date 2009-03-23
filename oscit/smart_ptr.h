#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_
#include <stdlib.h> // size_t

/** The SmartPtr class implements a simple Smart Pointer with reference counting. */
template <class T>
class SmartPtr
{
public:
  SmartPtr (T * p = 0) : ptr_(0)
  {
    if (p) ptr_ = new Ptr(p);
  }
  
  /** Initialize with another SmartPtr: light copy of content. */
  SmartPtr(const SmartPtr& other)
  {
    acquire(other.ptr_);
  }
  
  /** Light copy of the data contained in the other SmartPtr. */
  SmartPtr& operator=(const SmartPtr& v)
  {
    if (this != &v) {
      release();
      acquire(v.ptr_);
    }
    return *this;
  }
  
  /** When the SmartPtr dies, it releases the content (decrement refcount). */
  virtual ~SmartPtr()
  {
    release();
  }
  
  T * getData()
  {
    return (ptr_ ? ptr_->dataPtr_ : NULL);
  }
  
  /** Return reference count for the content. Only used for testing/debugging. */
  size_t getRefCount() const
  {
    return ptr_ ? ptr_->refCount_ : 0;
  }
  
  void acquire(const SmartPtr& s)
  {
    acquire(s.ptr_);
  }
protected:
  
  /** Structure shared by all smart pointers pointing to the same data. Holds the pointer to the "content" and the reference count. */
  struct Ptr 
  {
    Ptr(T* p = 0, size_t c = 1) : dataPtr_(p), refCount_(c) {}
    T * dataPtr_;
    size_t refCount_;
  };
  
  Ptr * ptr_;
  
private:
  
  /** Get hold of a pointer (the SmartPtr is in charge of calling 'delete'). */
  void acquire(Ptr * p)
  {  
    ptr_ = p;
    if (ptr_) {
      ptr_->refCount_++;
    }
  }
  
  /** Decrement reference counter, destroying content if the reference count drops to zero. */
  void release()
  {
    if (ptr_ && --ptr_->refCount_ == 0) {
      delete ptr_->dataPtr_;
      delete ptr_;
    }
    // stop pointing to counter+data
    ptr_ = NULL;
  }

};

#endif // _SMART_PTR_H_