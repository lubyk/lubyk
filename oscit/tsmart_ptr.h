#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_
#include <stdlib.h> // size_t

/** The TSmartPtr class implements a simple Smart Pointer with reference counting. */
template <class T>
class TSmartPtr
{
public:
  TSmartPtr (T * p = 0) : ptr_(0)
  {
    if (p) ptr_ = new Ptr(p);
  }
  
  /** Initialize with another TSmartPtr: light copy of content. */
  TSmartPtr(const TSmartPtr& other)
  {
    acquire(other.ptr_);
  }
  
  /** Light copy of the data contained in the other TSmartPtr. */
  TSmartPtr& operator=(const TSmartPtr& v)
  {
    if (this != &v) {
      release();
      acquire(v.ptr_);
    }
    return *this;
  }
  
  /** When the TSmartPtr dies, it releases the content (decrement refcount). */
  virtual ~TSmartPtr()
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
  
  void acquire(const TSmartPtr& s)
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
  
  /** Get hold of a pointer (the TSmartPtr is in charge of calling 'delete'). */
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