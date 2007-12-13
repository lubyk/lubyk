#include "class.h"
#include "buffer.h"

#define DEFAULT_WINDOW_COUNT 5

class BufferNode : public Node
{
public:
  
  bool init (const Params& p)
  {
    if(!mBuffer.set_sizes(32, 8)) return false;
    mVector   = mBuffer.advance();
    mColCount = mBuffer.col_count();
    mIndex    = 0;
    return true;
  }
  
  
  bool set (const Params& p)
  {
    size_t window_size = mBuffer.row_count();
    size_t vector_size = mBuffer.col_count();
    if (p.get(&window_size, "buffer", true) || p.get(&vector_size, "vector")) {
      if (!mBuffer.set_sizes(window_size, vector_size)) {
        *mOutput << mName << ": " << mBuffer.error_msg() << std::endl;
        return false;
      }
      mVector   = mBuffer.advance();
      mColCount = vector_size;
      mIndex    = 0;
    }
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