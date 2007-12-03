/** Smart and fast buffer. Providing direct access via pointers (at the programmer's own risk). */

#ifndef _BUF_H_
#define _BUF_H_

#define BUF_INITIAL_SIZE 16
#define BUF_MAX_ERROR_MSG_SIZE 200

union Signal;

// K is the key class, T is the object class
template <typename T>
class Buf
{
public:
  Buf() :  mStorageSize(0), mSize(0), data(NULL)
  {
    mErrorMsg = "no error";
  }

  virtual ~Buf()
  {
    if (data) free(data);
  }
  
  /** Set storage size (allocate memory). */
  bool set_storage(size_t pSize)
  {
    mSize = 0;
    if (pSize > mStorageSize)
      return check_alloc(pSize);
    else
      return reallocate(pSize);
  }
  
  /** Set all values to 'pVal'. */
  void fill(T& pVal)
  {
    for(int i=0; i < mStorageSize; i++)
      data[i] = pVal;
    mSize = 0;
  }
  
  bool copy(T* pVector, size_t pVectorSize)
  {
    mSize = 0;
    return append(pVector, pVectorSize);
  }
  
  bool copy(const Buf<T>& pBuf)
  {
    mSize = 0;
    return append(pBuf.data, pBuf.mSize);
  }
  
  bool copy(const Signal& sig); // implemented for <double> buffer in rubyk_signal.h
  
  /** Append to the end of the current data. Size becomes size+1. */
  bool append(T pValue)
  {
    if(!check_alloc(mSize + 1)) return false;
    data[mSize] = pValue;
    mSize++;
    return true;
  }
  
  /** Append a vector to the end of the current data. Size becomes size + pVectorSize. */
  bool append(T* pVector, size_t pVectorSize)
  {
    if(!check_alloc(mSize + pVectorSize)) return false;
    // use memcpy
    memcpy(data + mSize, pVector, pVectorSize * sizeof(T));
    mSize += pVectorSize;
    return true;
  }
  
  bool append(const Buf<T>& pBuf)
  {
    return append(pBuf.data, pBuf.mSize);
  }
  
  Buf<T>& operator-= (T& pValue)
  {
    for(int i=0; i < mSize; i++) {
      data[i] -= pValue;
    }
  }
  
  /** Return the size of the current buffer (may be different from the storage size). Accessor optimized away with (O1 flag). */
  const size_t& size()
  {
    return mSize;
  }
public:
  T * data; /**< Pointer to the data. Public for performance, handle with care. */

protected:
  size_t mSize;
  const char * mErrorMsg;
  char mErrorBuffer[BUF_MAX_ERROR_MSG_SIZE];
  
private:
  inline bool check_alloc(size_t pSize)
  {
    if (!mStorageSize) {
      size_t storage;
      if (pSize < BUF_INITIAL_SIZE)
        storage = BUF_INITIAL_SIZE;
      else
        storage = pSize;
      
      data = (T*)malloc(storage * sizeof(T));
      if (!data) {
        snprintf(mErrorBuffer, BUF_MAX_ERROR_MSG_SIZE, "could not allocate %ix%i", storage, sizeof(T));
        mErrorMsg = mErrorBuffer;
        return false;
      }
      mStorageSize = storage;
    } else if (pSize > mStorageSize) {
      return reallocate(mStorageSize * 2);
    }  
    return true;
  }
  
  inline bool reallocate(size_t pSize)
  {
    T * tmp = (T*)realloc(data, pSize * sizeof(T));
    if (!tmp) {
      snprintf(mErrorBuffer, BUF_MAX_ERROR_MSG_SIZE, "could not reallocate %ix%i to %ix%i", mStorageSize, sizeof(T), pSize, sizeof(T));
      return false;
    }
    data = tmp;
    mStorageSize = pSize;
    if (mSize > mStorageSize) mSize = mStorageSize;
    return true;
  }
  
  size_t mStorageSize;
};

#endif // _BUF_H_