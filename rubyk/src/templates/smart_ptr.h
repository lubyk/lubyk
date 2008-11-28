#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_
#include <stdlib.h> // size_t

/** The SmartPtr class implements a simple Smart Pointer with reference counting and copy-on-write. It is intentional that this class does not contain any dereferencement mechanism. These should be implemented in sub-classes. This enables us to have a generic "wrapper" which can only be used after type conversion. */
template <class T>
class SmartPtr
{
public:
  SmartPtr (T * p = 0) : mPtr(0)
  {
    if (p) mPtr = new Ptr(p);
  }
  
  /** Initialize with another SmartPtr: light copy of content. */
  SmartPtr(const SmartPtr& pOther)
  {
    acquire(pOther.mPtr);
  }
  
  /** Light copy of the data contained in the other SmartPtr. */
  SmartPtr& operator=(const SmartPtr& v)
  {
    if (this != &v) {
      release();
      acquire(v.mPtr);
    }
    return *this;
  }
  
  /** When the SmartPtr dies, it releases the content (decrement refcount). */
  virtual ~SmartPtr()
  {
    release();
  }
  
  /** Return reference count for the content. Only used for testing/debugging. */
  size_t ref_count() const
  {
    return mPtr ? mPtr->mRefCount : 0;
  }
  
protected:
  
  /** Structure shared by all smart pointers pointing to the same data. Holds the pointer to the "content" and the reference count. */
  struct Ptr 
  {
    Ptr(T* p = 0, size_t c = 1) : mDataPtr(p), mRefCount(c) {}
    T * mDataPtr;
    size_t mRefCount;
  };
  
  /** Create a (writteable) copy if the content is shared. */
  inline void copy_if_shared()
  {
    if (mPtr->mRefCount > 1) copy();
  }
  
  /** Copy before alteration. Only called if the content is not shared. */
  void copy()
  {
    if (mPtr) {
      Ptr * p = new Ptr(NULL,0);
      p->mDataPtr = mPtr->mDataPtr->clone(); // copy content
      release();  // release old content
      acquire(p); // acquire new copy
    }
  }
  
  Ptr * mPtr;
  
private:
  
  /** Get hold of a pointer (the SmartPtr is in charge of calling 'delete'). */
  void acquire(Ptr * p)
  {  
    mPtr = p;
    if (mPtr) {
      mPtr->mRefCount++;
    }
  }
  
  /** Decrement reference counter, destroying content if the reference count drops to zero. */
  void release()
  {
    if (mPtr && --mPtr->mRefCount == 0) {
      delete mPtr->mDataPtr;
      delete mPtr;
    }
    // stop pointing to counter+data
    mPtr = 0;
  }

};

#endif // _SMART_PTR_H_