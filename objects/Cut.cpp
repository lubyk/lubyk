#include "class.h"

class Cut : public Node
{
public:
  
  bool init (const Params& p)
  {
    mCutFrom = p.val("from", 0);
    mCutTo   = p.val("to", 32);
    
    mS.type = ArraySignal;
    return mCutTo >= mCutFrom && mCutFrom >= 0;
  }

  void bang(const Signal& sig)
  { 
    if(sig.get(&mLiveBuffer)) {
      mLiveBufferSize = sig.array.size;
    }
    
    if (mCutFrom < mLiveBufferSize) {
      mS.array.value = mLiveBuffer + mCutFrom;
      if (mCutTo < mLiveBufferSize) {
        mS.array.size = mCutTo - mCutFrom + 1;
      } else {
        mS.array.size = mLiveBufferSize - mCutFrom;
      }
      
      if (mDebug)
        *mOutput << mName << ": " << mS << std::endl;
      send(mS);
    } else {
      if (mDebug)
        *mOutput << mName << ": " << gNilSignal << std::endl;
    }
  }
  
private:
  double * mLiveBuffer;      /** Live input stream. */
  int      mLiveBufferSize;  /** Original size of the input stream. */
  int      mCutFrom;     /** First value in output buffer. */
  int      mCutTo;       /** Last value in output buffer.  */
};

extern "C" void init()
{
  CLASS (Cut)
  OUTLET(Cut, cut)
}