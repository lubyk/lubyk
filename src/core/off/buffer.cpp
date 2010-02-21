/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "buffer.h"

#define DEFAULT_WINDOW_COUNT 5

Buffer::Buffer (size_t pRowCount, size_t pColCount)
{
  set_sizes(pRowCount, pColCount);
  mWriteRow = 0;
}

bool Buffer::set_sizes(size_t pRowCount, size_t pColCount)
{
  if (!mBuffer.set_sizes(pRowCount * DEFAULT_WINDOW_COUNT, pColCount)) {
    mErrorMsg = mBuffer.error_msg();
    return false;
  }
  
  mWindowSize = pRowCount;
  mWriteRow = mWindowSize;
  if (!mWindow.set_sizes(mWindowSize, mBuffer.col_count())) {
    mErrorMsg = mWindow.error_msg();
    return false;
  }
  
  clear();
  return true;
}

void Buffer::clear()
{
  mBuffer.clear();
}

/** Move read/write position forward. 
  *
  * @return pointer to the write position for the next vector. */
Real * Buffer::advance()
{
  if (!mWindowSize) return NULL;
  if (mWriteRow >= (mWindowSize * (DEFAULT_WINDOW_COUNT - 1) + 1)) {
    // copy vector for loop head
    if(!mBuffer.copy_at((int)(mWriteRow - mWindowSize * (DEFAULT_WINDOW_COUNT - 1) - 1), mBuffer, mWriteRow, mWriteRow)) {
      printf("could not copy. (%s)\n", mBuffer.error_msg());
      return NULL;
    }

  }
  
  mWriteRow++;
  mWindow.data = mBuffer[mWriteRow - mWindowSize];
  
  if (mWriteRow >= (mWindowSize * DEFAULT_WINDOW_COUNT))
    mWriteRow = mWindowSize - 1;
  
  return mBuffer[mWriteRow];
}