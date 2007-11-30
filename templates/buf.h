/** Smart and fast buffer. Providing direct access via pointers (at the programmer's own risk). */

#ifndef _BUF_H_
#define _BUF_H_

#define BUF_INITIAL_STORAGE_SIZE 16

union Signal;

// K is the key class, T is the object class
template <typename T>
class Buf
{
public:
  Buf() :  storage_size(0), size(0), data(NULL) {}
  virtual ~Buf()
  {
    if (data) free(data);
  }
  /** Set storage_size. */
  bool set_storage_size(size_t pSize)
  {
    if (pSize > storage_size)
      return check_alloc(pSize);
    else
      return reallocate(pSize);
  }
  
  /** Set all values to 'pVal'. */
  void clear(T& pVal)
  {
    for(int i=0; i < storage_size; i++)
      data[i] = pVal;
    size = 0;
  }
  
  bool set(T* pVector, size_t pVectorSize)
  {
    size = 0;
    return append(pVector, pVectorSize);
  }
  
  bool set(const Buf<T>& pBuf)
  {
    size = 0;
    return append(pBuf.data, pBuf.size);
  }
  
  bool set(const Signal& sig); // implemented for <double> buffer in rubyk_signal.h
  
  /** Append to the end of the current data. Size becomes size+1. */
  bool append(T pValue)
  {
    if(!check_alloc(size + 1)) return false;
    data[size] = pValue;
    size++;
    return true;
  }
  
  /** Append a vector to the end of the current data. Size becomes size + pVectorSize. */
  bool append(T* pVector, size_t pVectorSize)
  {
    if(!check_alloc(size + pVectorSize)) return false;
    for(int i=0; i < pVectorSize; i++)
      data[size + i] = pVector[i];
    size += pVectorSize;
    return true;
  }
  
  bool append(const Buf<T>& pBuf)
  {
    return append(pBuf.data, pBuf.size);
  }
  
  Buf<T>& operator-= (T& pValue)
  {
    for(int i=0; i < size; i++) {
      data[i] -= pValue;
    }
  }
  
public:
  size_t size;
  T * data;
  
private:
  inline bool check_alloc(size_t pSize)
  {
    if (!storage_size) {
      size_t storage;
      if (pSize < BUF_INITIAL_STORAGE_SIZE)
        storage = BUF_INITIAL_STORAGE_SIZE;
      else
        storage = pSize;
      
      data = (T*)malloc(storage * sizeof(T));
      if (!data) return false;
      storage_size = storage;
    } else if (pSize > storage_size) {
      return reallocate(storage_size * 2);
    }  
    return true;
  }
  
  inline bool reallocate(size_t pSize)
  {
    T * tmp = (T*)realloc(data, pSize * sizeof(T));
    if (!tmp) return false;
    data = tmp;
    storage_size = pSize;
    if (size > storage_size) size = storage_size;
    return true;
  }
  
  size_t storage_size;
};

#endif // _BUF_H_