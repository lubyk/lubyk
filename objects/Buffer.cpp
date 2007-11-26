#include "class.h"

#define DEFAULT_WINDOW_COUNT 5

class Buffer : public Node
{
public:
  
  ~Buffer ()
  {
    if (mBuffer) free(mBuffer);
  }
  
  bool init (const Params& p)
  {
    return init_buffer(p);
  }
  
  bool init_buffer (const Params& p)
  {
    if (!p.get(&mWindowSize, "buffer", true)) mWindowSize = 32;
    
    // use buffer
    size_t sz = mWindowSize * DEFAULT_WINDOW_COUNT; // write twice once every 10 windows
    mBuffer = (double*)malloc(sz * sizeof(double));
    if (!mBuffer) {
      *mOutput << "Could not allocate '" << sz << "' doubles.\n";
      return false;
    } else {
      for(int i= 0;i<mWindowSize;i++)
        mBuffer[i] = 0.0;
      mNextWrite = mWindowSize - 1;
      mReadPosition = 0;
      mReadOffset   = 0;
    }
    
    mS.type = ArraySignal;
    mS.array.size  = mWindowSize;
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (!mIsOK) return;
    switch(sig.type) {
    case ArraySignal:
      for(int i=0; i < sig.array.size;i++)
        write_buf(*(sig.array.value + i));
      break;
    default:
      double d;
      if (sig.get(&d)) write_buf(d);
    }
    mS.array.value = mBuffer + mReadPosition;
    if (mDebug)
      *mOutput << mName << ": " << mS << std::endl;
    send(mS);
  }

  inline void write_buf(double f)
  {
    mBuffer[mNextWrite] = f;
    
    if (mNextWrite >= (mWindowSize * (DEFAULT_WINDOW_COUNT - 1))) {
      // copy value to loop head 
      mBuffer[mNextWrite - (mWindowSize * (DEFAULT_WINDOW_COUNT - 1))] = f;
    }
    
    mReadPosition = mNextWrite - mWindowSize + 1 - mReadOffset;
    
    if (mReadPosition < 0)
      mReadPosition = (mWindowSize * (DEFAULT_WINDOW_COUNT - 1)) + mReadPosition;
    
    mNextWrite++;
    
    if (mNextWrite >= (mWindowSize * DEFAULT_WINDOW_COUNT)) {
      mNextWrite = mWindowSize;
    }
    
  }
  
private:
  double * mBuffer;
  int      mWindowSize;
  int      mNextWrite;
  int      mReadPosition;
  int      mReadOffset;
};

extern "C" void init()
{
  CLASS (Buffer)
  OUTLET(Buffer,data)
}