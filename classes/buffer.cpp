#include "buffer.h"

#define DEFAULT_WINDOW_COUNT 5

Buffer::Buffer (size_t pRowCount, size_t pColCount)
{
  set_sizes(pRowCount, pColCount);
}

bool Buffer::set_sizes(size_t pRowCount, size_t pColCount)
{
  if (!mBuffer.set_sizes(pRowCount * DEFAULT_WINDOW_COUNT, pColCount)) return false;
  mWindowSize = pRowCount;
  mWriteRow = mWindowSize;
  mWindow.set_sizes(mWindowSize, mBuffer.col_count());
  return true;
}

void Buffer::clear()
{
  mBuffer.clear();
}

/** Move read/write position forward. 
  *
  * @return pointer to the write position for the next vector. */
double * Buffer::advance()
{
  
  if (mWriteRow >= (mWindowSize * (DEFAULT_WINDOW_COUNT - 1))) {
    // copy vector for loop head
    mBuffer.copy_at(mWriteRow - (mWindowSize * (DEFAULT_WINDOW_COUNT - 1)), mBuffer, mWriteRow, mWriteRow);
  }
  
  mWriteRow++;
  if (mWriteRow >= (mWindowSize * DEFAULT_WINDOW_COUNT))
    mWriteRow = mWindowSize;
  
  mWindow.data = mBuffer[mWriteRow - mWindowSize];
  return mBuffer[mWriteRow];
}