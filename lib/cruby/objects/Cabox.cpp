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
    mValueCount = 0;
    mRate       = 0.0;
    mStart      = mServer->mCurrentTime;
    
    mIndex = 0;
    mReadPosition = 12;
    mState = -3; // wait for sync
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
    mNewData = false;
    while (mPort.read_char(&c)) {
      // empty buffer (readall)
      //printf("% i:%i [%i]\n", mState, mIndex, c);
      
      if (mState == -3) {
        if (c == 255) mState++;
        continue;
      }
    
      if (mState == -2) { // first sync received
        if (c == 255) { // move into data mode
          mState++;
          continue;
        } else
          mState--;
        continue;
      }
    
      if (mState == -1) {
        mHigh = c;
        mState++;
        continue;
      }
      
      val  = (mHigh * 256) + c;
      val  = (val/1024) * (3300 - 1650) / 200; // 3300 = fullscale_mv, 1024 = fullscale_bin, 1650 = offset_mv, 200 = sensitivity_mv
      
      val -= mOffset[mIndex % 12];
      mValues[mIndex] = val;
      //printf("val[%i] = %.2f  [%i,%i]\n",mIndex,val,mHigh,c);
      
      float abs_val;
      
      abs_val = val < 0 ? -val : val;
      // abs(..) casts to integer ????
      
      if ( abs_val > mFindHighestValue ) {
        mFindHighestDirection = mIndex;
        mFindHighestValue = abs_val;
      }
      
      mIndex++;
      if (mIndex == 12 || mIndex >= 24) {
        mValueCount++;
        mState = -3; // wait for sync
        mNewData = true;
        mHighestValue         = mFindHighestValue;
        mHighestDirection     = mFindHighestDirection;
        mFindHighestValue     = 0.0;
        mFindHighestDirection = 0;
        if (mIndex == 12) {
          mReadPosition = 0;
        } else {  
          mIndex = 0; // restart
          mReadPosition = 12;
          if (mValueCount > 800) {
            mRate  = mValueCount * 1000.0 / (mServer->mCurrentTime - mStart);
            mValueCount = 0;
            mStart = mServer->mCurrentTime;
          }
        }
        
        if (mOffsetOnFull) {
          for(int i=0; i<12; i++) {
            mOffset[i] = mValues[mReadPosition + i] + mOffset[i];
          }
          mOffsetOnFull = false;
        }
      } else {
        mState--;
      }
    }
    
    if (mNewData) {
      
      // outlet 3 (highest direction)
      send(mHighestDirection, 3);
      
      // outlet 2 (highest value)
      send(mHighestValue, 2);
      
      // outlet 1 (stream)
      mDataSignal.array.value = mValues + mReadPosition;
      
      if (mDebug)
        *mOutput << mDataSignal << std::endl;
        
      // this is just to debug Plot [DEBUG
      //mDataSignal.type = DoubleSignal;
      //mDataSignal.d.value = mValues[mReadPosition];
      // DEBUG]
      
      send(mDataSignal);
      
    } 
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
  void spy() 
  { bprint(mSpy, mSpySize,"%.2f /s", mRate );  }
  
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
  
  long int mValueCount;
  double mRate;
  time_t mStart;
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