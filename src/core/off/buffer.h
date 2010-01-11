/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "tmatrix.h"

/** The Buffer class gives a buffered 'window' on streaming data. Every new vector
  * that is appended to the matrix becomes the most recent (index -1) and pushes the
  * previous ones back. The buffer is implemented using a sliding window to minimize copying
  * of the data. */
class Buffer
{
public:
  Buffer () { mErrorMsg = ""; }
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
  Real * advance ();
  
  /** Return a (read-only) matrix for the current window. */
  const CutMatrix& matrix() const
  {
    return mWindow;
  }
  
  /** Return last error message. */
  const char * error_msg()
  {
    return mErrorMsg;
  }
  
  size_t col_count()
  {
    return mBuffer.col_count();
  }
  
  size_t row_count()
  {
    return mWindowSize;
  }
  
  size_t size()
  {
    return mWindowSize * mBuffer.col_count();
  }

private:
  const char * mErrorMsg; /**< Used to display errors from mBuffer or mWindow. */
  Matrix mBuffer;      /**< Contains N x window matrices. */
  CutMatrix mWindow;   /**< Current view on the data. */
  size_t mWindowSize;  /**< Size of the window (number of rows). */
  size_t mWriteRow;    /**< Current row to write the data. */
};

#endif // _BUFFER_H_