#include "class.h"

#define DEFAULT_WINDOW_COUNT 5

class Pack : public Node
{
public:
  
  ~Pack ()
  {
    if (mBuffer) free(mBuffer);
  }
  
  bool init (const Params& p)
  {
    mPackSize = p.val("pack", 4);
    
    if (mPackSize) {
      mBuffer = (double*)malloc(2 * mPackSize * sizeof(double));
      if (!mBuffer) {
        *mOutput << "Could not allocate '" << (2 * mPackSize) << "' array.\n";
      } else {
        for(int i= 0;i<(2*mPackSize);i++)
          mBuffer[i] = 0.0;
      }
      
      mS.type = ArraySignal;
      mS.array.size  = mPackSize;
      mReadPosition = mPackSize;
      mIndex        = 0;
    }
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (mBuffer) {
      switch(sig.type) {
      case ArraySignal:
        for(int i=0; i < sig.array.size;i++)
          pack(*(sig.array.value + i));
        break;
      default:
        double d;
        if (sig.get(&d)) pack(d);
      }
    } else
      send(sig);
  }

  inline void pack(float f)
  {
    if (!mBuffer) {
      *mOutput << "Serial error: trying to write to buffer without window.\n";
      return;
    }
    mBuffer[mIndex] = f;
    mIndex++;
    
    if (mIndex == mPackSize) {
      mReadPosition = 0;
      mS.array.value = mBuffer + mReadPosition;
      send(mS);
    } else if (mIndex >= 2 * mPackSize) {
      mIndex = 0;
      mReadPosition = mPackSize;
      mS.array.value = mBuffer + mReadPosition;
      send(mS);
    }
  }
  
private:
  double * mBuffer;
  int      mPackSize;
  int      mIndex;
  int      mReadPosition;
};

extern "C" void init()
{
  CLASS (Pack)
  OUTLET(Pack,data)
}