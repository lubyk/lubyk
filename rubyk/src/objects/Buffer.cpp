#include "class.h"
#include "buffer.h"

#define DEFAULT_WINDOW_COUNT 5

class BufferNode : public Node
{
public:
  
  bool init (const Params& p)
  {
    if(!mBuffer.set_sizes(8, 0)) return false;
    mVector   = NULL;
    mIndex    = 0;
    return true;
  }
  
  // TODO: add 'flat' option.
  bool set (const Params& p)
  {
    size_t window_size = mBuffer.row_count();
    size_t vector_size = mBuffer.col_count();
    if (p.get(&window_size, "buffer", true)) {
      TRY(mBuffer, set_sizes(window_size, mBuffer.col_count()));
    } 
    
    if (p.get(&vector_size, "vector")) {
      TRY(mBuffer, set_sizes(mBuffer.row_count(), vector_size));
    }
    
    if (mBuffer.size()) mVector = mBuffer.advance();
    mIndex  = 0;
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {   
    double d;
    if (!mIsOK) return;
    if(sig.type == MatrixSignal) {
      double * data = sig.matrix.value->data;
      if (!mVector) {
        // get buffer size from incoming signal
        TRY_RET(mBuffer, set_sizes(mBuffer.row_count(), sig.matrix.value->col_count()));
        mVector = mBuffer.advance();
        if (!mVector) return;
        mIndex  = 0;
      }
      for(size_t i=0; i < sig.matrix.value->col_count(); i++) {
        mVector[mIndex] = data[i];
        mIndex++;
        if (mIndex >= mBuffer.col_count()) {
          mVector = mBuffer.advance();
          mIndex = 0;
        }
      }
    } else if (sig.get(&d)) {
      if (!mVector) {
        // get buffer size from incoming signal
        TRY_RET(mBuffer, set_sizes(mBuffer.row_count(), 1));
        mVector = mBuffer.advance();
        if (!mVector) return;
        mIndex  = 0;
      }
      mVector[mIndex] = d;
      mIndex++;
      if (mIndex >= mBuffer.col_count()) {
        mVector = mBuffer.advance();
        mIndex = 0;
      }
    }
    mS.set(mBuffer.matrix());
    send(mS);
  }
  
  virtual void spy() 
  { 
    bprint(mSpy, mSpySize,"%ix%i", mBuffer.row_count(), mBuffer.col_count() );
  }
private:
  
  Buffer mBuffer;
  double * mVector; /**< Vector pointing into mBuffer to write the data. */
  size_t   mIndex;  /**< Current write position into the 'write' vector. */
};

extern "C" void init()
{
  CLASS_NAME (Buffer,BufferNode)
  OUTLET     (Buffer,data)
}