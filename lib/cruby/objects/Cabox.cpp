#include "Serial.cpp"

class Cabox : public Serial
{
public:
  
  bool init (const Params& p)
  {
    for(int i=0; i<24;i++) {
      mValues[i] = 0.0;
      mOffset[i] = 0.0;
    }
    
    mDebug = false;
    mNewData = false;
    
    mHighestValue = 0.0;
    mHighestDirection = 0;
    mFindHighestValue = 0.0;
    mFindHighestDirection = 0;
    
    mIndex = 0;
    mReadPosition = 12;
    mState = 0; // wait for sync
    mOffsetOnFull = true; // do not offset when 12 values are set
    
    mDataSignal.type = ArraySignal;
    mDataSignal.array.size  = 12;
    
    if (init_serial(p))
    {
      // enter read data
      return mPort.write_char('b');
    } else
      return false;
  }
  
  
  void bang(const Signal& sig)
  {
    int c,r;
    double val;
    int i;
    bool read_count = 0;
    mNewData = false;
    while (mPort.read_char(&c)) {
      // empty buffer (readall)
      read_count++;
      
      //printf("%i[%i]\n",mState,c);
      if (mState == 0) {
        if (c == 255) mState = 1;
        continue;
      }
    
      if (mState == 1) { // first sync received
        if (c == 255) { // move into data mode
          mIndex = 0;
          mState = 2;
          continue;
        } else
          mState = 0;
        continue;
      }
    
      if (mState == 2 && ((mIndex % 2) == 0)) {
        mIndex += 1;
        mHigh = c;
        continue;
      }
    
      val  = (mHigh * 256) + c;
      val  = (val/1024) * (3300 - 1650) / 200; // 3300 = fullscale_mv, 1024 = fullscale_bin, 1650 = offset_mv, 200 = sensitivity_mv
      mIndex += 1;
      i = (mIndex/2) - 1;
      val -= mOffset[i];
      mValues[i] = val;
      
      float abs_val;
      
      abs_val = val < 0 ? -val : val;
      // abs(..) casts to integer ????
      
      if ( abs_val > mFindHighestValue ) {
        mFindHighestDirection = i;
        mFindHighestValue = abs_val;
      }
      
      if (i == 11 || i >= 23) {
        mNewData = true;
        mHighestValue         = mFindHighestValue;
        mHighestDirection     = mFindHighestDirection;
        mFindHighestValue     = 0.0;
        mFindHighestDirection = 0;
        if (mOffsetOnFull) {
          for(int i=0; i<12; i++) {
            mOffset[i] = mValues[i] + mOffset[i];
          }
          mOffsetOnFull = false;
        }
        mState = 0;
        if (i == 11) {
          mReadPosition = 0;
        } else {
          mReadPosition = 12;
          mIndex = 0;
        }
      }
    }
    
    if (mNewData) {
      
      // outlet 3 (highest direction)
      send(mHighestDirection, 3);
      
      // outlet 2 (highest value)
      send(mHighestValue, 2);
      
      // outlet 1 (stream)
      mDataSignal.array.value = mValues + mReadPosition;
      send(mDataSignal);
      
      if (mDebug)
        *mOutput << mDataSignal << std::endl;
    } 
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
  void debug()
  {
    mDebug = !mDebug;
    if (mDebug)
      *mOutput << mName << ": debug on\n";
    else
      *mOutput << mName << ": debug off\n";
  }
  
private:

  bool   mDebug;
  bool   mNewData;
  Signal mDataSignal;
  
  double mValues[24];
  int    mReadPosition;
  double mOffset[12];
  double mHighestValue;
  int mHighestDirection;
  double mFindHighestValue;
  int mFindHighestDirection;
  int mIndex;
  int mState;
  int mHigh;
  bool mOffsetOnFull;
};

extern "C" void init()
{
  CLASS (Cabox)
  OUTLET(Cabox,stream)
  OUTLET(Cabox,high)
  OUTLET(Cabox,direction)
  METHOD(Cabox,offset)
  METHOD(Cabox,debug)
  SUPER_METHOD(Cabox, Script, set)
  SUPER_METHOD(Cabox, Script, load)
  SUPER_METHOD(Cabox, Script, script)
}