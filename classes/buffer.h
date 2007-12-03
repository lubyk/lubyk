#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "matrix.h"

/** The Buffer class gives a buffered 'window' on streaming data. Every new vector
  * that is appended to the matrix becomes the most recent (index -1) and pushes the
  * previous ones back. The buffer is implemented using a sliding window to minimize copying
  * of the data. */
class Buffer
{
public:
  Buffer () {}
  Buffer (size_t pRowCount, size_t pColCount);
  
  /** Set row/column count and window size.
    * @param pRowCount number of rows we want to keep in the buffer.
    * @param pColCount size of a vector.
    * @return false in case of memory allocation failure. */
  bool set_sizes (size_t pRowCount, size_t pColCount);
  
  /** Clear all values to 0. */
  void clear ();
  
  /** Move read/write position forward. 
    *
    * @return pointer to the write position (inside mBuffer) for the next vector. */
  double * advance ();
  
  /** Return a (read-only) matrix for the current window. */
  const CutMatrix& matrix() const
  {
    return mWindow;
  }
  
  /** Return last error message. */
  const char * error_msg()
  {
    return mBuffer.error_msg();
  }

private:
  Matrix mBuffer;      /**< Contains N x window matrices. */
  CutMatrix mWindow;   /**< Current view on the data. */
  size_t mWindowSize;  /**< Size of the window (number of rows). */
  size_t mWriteRow;    /**< Current row to write the data. */
};

#endif // _BUFFER_H_