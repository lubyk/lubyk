#include "rubyk.h"

class Peak : public Node
{
public:
  
  bool set (const Value& p)
  {
    mDescent = p.val("descent", 0.125, true);
    mS.set(mBuffer);
    return true;
  }

  void bang(const Value& val)
  { 
    real_t d = -1;
    size_t row_count, col_count;
    const Matrix * live_buffer = NULL;
    if(sig.get(&live_buffer)) {
      row_count = live_buffer->row_count();
      col_count = live_buffer->col_count();
    } else if (sig.get(&d)) {
      d = absval(d);
      row_count = 1;
      col_count = 1;
    } else {
      row_count = mBuffer.row_count();
      col_count = mBuffer.col_count();
    }
    
    if (mBuffer.row_count() != row_count || mBuffer.col_count() != col_count) {
      if(!mBuffer.set_sizes(row_count, col_count)) {
        *mOutput << mName << ": " << mBuffer.error_msg() << std::endl;
        return;
      }
      mBuffer.clear();
    }
    
    for(size_t i=0; i < row_count * col_count; i++) {
      if (live_buffer)
        d = absval(live_buffer->data[i]);
        
      // 1. descent
      mBuffer.data[i] -= mDescent;
      if (mBuffer.data[i] < 0) mBuffer.data[i] = 0;
      
      // 2. peak
      if (d > mBuffer.data[i]) mBuffer.data[i] = d; // d == -1 when there is no new signal.
    }
    
    if (row_count == 1 && col_count == 1) {
      send(mBuffer.data[0]);
    } else {
      send(mS);
    }
  }
  
  virtual const Value inspect(const Value& val) 
  { bprint(mSpy, mSpySize,"%.2f", mDescent );  }
  
private:
  Matrix mBuffer;      /** Current peak values. */
  real_t mDescent;     /** Peak decreasing speed in value/sample. */
};

extern "C" void init()
{
  CLASS (Peak)
  OUTLET(Peak, peak)
}