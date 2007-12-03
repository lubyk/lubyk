#include "class.h"
#include "buffer.h"

#define DEFAULT_WINDOW_COUNT 5

class BufferNode : public Node
{
public:
  
  bool set (const Params& p)
  {
    size_t window_size = p.val("buffer", 32, true);
    size_t vector_size = p.val("vector", 8);
    if (!mBuffer.set_sizes(window_size, vector_size)) {
      *mOutput << mName << ": " << mBuffer.error_msg() << std::endl;
      return false;
    }
    mVector   = mBuffer.advance();
    mColCount = vector_size;
    mIndex    = 0;
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (!mIsOK) return;
    
    switch(sig.type) {
    case MatrixSignal:
      double * data = sig.matrix.value->data;
      for(size_t i=0; i < sig.matrix.value->col_count(); i++) {
        mVector[mIndex] = data[i];
        mIndex++;
        if (mIndex >= mColCount) {
          mVector = mBuffer.advance();
          mIndex = 0;
        }
      }
      break;
    default:
      double d;
      mVector[mIndex] = d;
      mIndex++;
      if (mIndex >= mColCount) {
        mVector = mBuffer.advance();
        mIndex = 0;
      }
    }
    mS.set(mBuffer.matrix());
    if (mDebug) *mOutput << mName << ": " << mS << std::endl;
    send(mS);
  }
  
private:
  Buffer mBuffer;
  double * mVector; /**< Vector pointing into mBuffer to write the data. */
  size_t   mIndex;  /**< Current write position into the 'write' vector. */
  size_t   mColCount; /**< Size of a vector. */
};

extern "C" void init()
{
  CLASS_NAME (Buffer,BufferNode)
  OUTLET     (Buffer,data)
}