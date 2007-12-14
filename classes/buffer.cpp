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
double * Buffer::advance()
{
  
  if (mWriteRow >= (mWindowSize * (DEFAULT_WINDOW_COUNT - 1) + 1)) {
    // copy vector for loop head
    if(!mBuffer.copy_at((int)(mWriteRow - mWindowSize * (DEFAULT_WINDOW_COUNT - 1) - 1), mBuffer, mWriteRow, mWriteRow)) {
      printf("could not copy. (%s)\n", mBuffer.error_msg());
    }

  }
  
  mWriteRow++;
  mWindow.data = mBuffer[mWriteRow - mWindowSize];
  
  if (mWriteRow >= (mWindowSize * DEFAULT_WINDOW_COUNT))
    mWriteRow = mWindowSize - 1;
  
  return mBuffer[mWriteRow];
}