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
    mWindowSize = p.val("buffer", 32);
    
    if (mWindowSize) {
      // use buffer
      size_t sz = mWindowSize * DEFAULT_WINDOW_COUNT; // write twice once every 10 windows
      mBuffer = (double*)malloc(sz * sizeof(double));
      if (!mBuffer) {
        *mOutput << "Could not allocate '" << sz << "' doubles.\n";
      } else {
        for(int i= 0;i<mWindowSize;i++)
          mBuffer[i] = 0.0;
        mNextWrite = mWindowSize - 1;
        mReadPosition = 0;
        mReadOffset   = 0;
      }
      
      mS.type = DoubleArraySignal;
      mS.doubles.size  = mWindowSize;
    }
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (mBuffer) {
      switch(sig.type) {
      case DoubleArraySignal:
        for(int i=0; i < sig.doubles.size;i++)
          write_buf(*(sig.doubles.value + i));
        break;
      default:
        double d;
        if (sig.get(&d)) write_buf(d);
      }
      mS.doubles.value = mBuffer + mReadPosition;
      send(mS);
    } else
      send(sig);
  }

  inline void write_buf(float f)
  {
    if (!mBuffer) {
      *mOutput << "Serial error: trying to write to buffer without window.\n";
      return;
    }
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