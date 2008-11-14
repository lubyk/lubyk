template <class T>
class SmartPtr
{
public:
  SmartPtr (T * p = 0) : mPtr(0)
  {
    if (p) mPtr = new Ptr(p);
  }
  
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
  }
  
  virtual ~SmartPtr()
  {
    release();
  }
  
  /** This could blow up if mPtr is NULL... */
  // template<class U>
  // const U* operator->() const
  // {
  //   return (U*)(mPtr->mDataPtr);
  // }
  // 
  // /** Return a const pointer to the data in the SmartPointer. */
  // template<class U>
  // const U * data() const
  // {
  //   return mPtr ? (U*)(mPtr->mDataPtr) : NULL;
  // }
  // 
  // /** Return a pointer to mutable data contained in the SmartPointer. Makes a copy if needed. */
  // template<class U>
  // U * mutable_data()
  // {
  //   if (!mPtr)
  //     return NULL;
  //     
  //   if (mPtr->mRefCount > 1)
  //     copy();
  //   
  //   return data();
  // }
  
  size_t ref_count() const
  {
    return mPtr ? mPtr->mRefCount : 0;
  }
  
protected:
  
  struct Ptr 
  {
    Ptr(T* p = 0, size_t c = 1) : mDataPtr(p), mRefCount(c) {}
    size_t mRefCount;
    T * mDataPtr;
  };
  
  void acquire(Ptr * p)
  {  
    mPtr = p;
    if (mPtr) {
      mPtr->mRefCount++;
    }
  }
  
  void release()
  {
    if (mPtr && --mPtr->mRefCount == 0) {
      delete mPtr->mDataPtr;
      delete mPtr;
    }
    // stop pointing to counter+data
    mPtr = 0;
  }
  
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
};

/*
class Data
{
public:
  Data()
  {
    printf
  }
  size_t size()
  { return mSize; }

private:
  size_t mSize;
};

class Matrix < Data
{
  virtual operator Matrix()
  { return *this; }
};

typedef SmartPtr<Data> Value;

void truc()
{
  Value v(new Matrix(10,10));
  bang(v); // count = 1
}

void Bang(Value& v)
{
  // const accessor, converted to string
  const String s = v;

  // non-const accessor, converted to matrix (copy)
  Matrix m = v.mutable();

  for(int i=0;i<2000;i++) {
    m.data[i] = i;
  }

  // conversion to double set to 1.0 (first value in matrix)
  double d = v; 
  Value v2 = v; // light copy

  printf("%d\n", v2->data[3]); // const accessor (no copy)


  const Matrix m2 = v2;  // no copy
  Matrix m3 = v2.mutable();  // makes a new copy (copy on write).
}
*/