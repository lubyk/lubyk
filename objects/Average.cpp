#include "class.h"

class Average : public Node
{
public:
  Average() : mBuffer(NULL) {}
  
  ~Average()
  {
    if (mBuffer) free(mBuffer);
  }
  
  bool init (const Params& p)
  {  
    mVectorSize   = p.val("vector", 32);    // number of values form a sample
    
    if (!alloc_doubles(&mBuffer, mVectorSize, "average buffer")) return false;
    
    mS.type = ArraySignal;
    mS.array.size = mVectorSize;
    mS.array.value = mBuffer;
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (sig.type == ArraySignal) {
      if (sig.array.size < mVectorSize) {
        *mOutput << mName << ": input stream too small (" << sig.array.size << ") should be " << mVectorSize << std::endl;
        return;
      }
      int val_count = (int)sig.array.size / mVectorSize;
      for(int i=0; i< mVectorSize; i++) {
        double sum = 0;
        for(int j=0; j< val_count; j++) {
          sum += sig.array.value[j*mVectorSize + i];
        }
        mBuffer[i] = sum / val_count;
      }
      send(mS);
      if (mDebug) {
        *mOutput << mName << ": " << mS << std::endl;
      }
    }
  }
private:
  double * mBuffer; /**< Average of incoming live stream. */
  int mVectorSize;
};

extern "C" void init()
{
  CLASS(Average)
  OUTLET(Average, average)
}