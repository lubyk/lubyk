#include "class.h"

class Crop : public Node
{
public:
  
  bool init (const Params& p)
  {
    if (!p.get(&mCropSize, "crop", true))
      mCropSize = 32;
    if (mCropSize < 0) {
      mCropSize = -mCropSize;
      mKeepRecent = true;
    } else {
      mKeepRecent = false;
    }
    
    return true;
  }

  void bang(const Signal& sig)
  { 
    if(sig.get(&mBuffer)) {
      mBufferSize = sig.array.size;
    } else {
      if (sig.get(&mCropSize)) {
        if (mCropSize < 0) {
          mCropSize   = -mCropSize;
          mKeepRecent = true;
        } else {
          mKeepRecent = false;
        }
      }
    }
    
    if (mCropSize <= mBufferSize) {
      if (mKeepRecent)
        mS.array.value = mBuffer + mBufferSize - mCropSize;
      else
        mS.array.value = mBuffer;
      
      mS.array.size = mCropSize;
    } else {
      mS.array.size  = mBufferSize;
      mS.array.value = mBuffer;
    };
    mS.type = ArraySignal;
    send(mS);
  }
  
private:
  double * mBuffer;
  int      mBufferSize;
  int      mCropSize;
  bool     mKeepRecent;
};

extern "C" void init()
{
  CLASS (Crop)
  OUTLET(Crop, data)
}