#include "class.h"

class Peak : public Node
{
public:
  Peak() : mBuffer(NULL) {}
  
  ~Peak()
  {
    if (mBuffer) free(mBuffer);
  }
  
  bool init (const Params& p)
  {
    mDescent = p.val("descent", 0.25);
    
    mBufferSize = 0;
    mS.type = ArraySignal;
    return true;
  }

  void bang(const Signal& sig)
  { 
    double d = -1;
    double * live_buffer = NULL;
    if(sig.get(&live_buffer)) {
      if (mBufferSize != sig.array.size) {
        if (!realloc_doubles(&mBuffer, sig.array.size, "peak buffer")) return;
        mBufferSize = sig.array.size;
        mS.array.value = mBuffer;
        mS.array.size  = mBufferSize;
        for(int i=0; i < mBufferSize; i++)
          mBuffer[i] = 0.0;
      }
    } else if (sig.get(&d)) {
      d = absval(d);
      // single value
      if (mBufferSize != 1) {
        if (!realloc_doubles(&mBuffer, 1, "peak buffer")) return;
        mBufferSize = 1;
        mBuffer[0] = 0.0;
      }
    }
    
    for(int i=0; i < mBufferSize; i++) {
      if (live_buffer)
        d = absval(live_buffer[i]);
        
      // 1. descent
      mBuffer[i] -= mDescent;
      if (mBuffer[i] < 0) mBuffer[i] = 0;

      // 2. peak
      if (d > mBuffer[i]) mBuffer[i] = d; // d == -1 when there is no new signal.
    }
    
    if (mBufferSize == 1) {
      if (mDebug) *mOutput << mName << ": " << mBuffer[0] << std::endl;
      send(mBuffer[0]);
    } else {  
      if (mDebug) *mOutput << mName << ": " << mS << std::endl;
      send(mS);
    }
  }
  
private:
  double * mBuffer;      /** Current peak values. */
  int      mBufferSize;  /** Size of the output stream. */
  double   mDescent;     /** Peak decreasing speed in value/sample. */
};

extern "C" void init()
{
  CLASS (Peak)
  OUTLET(Peak, peak)
}