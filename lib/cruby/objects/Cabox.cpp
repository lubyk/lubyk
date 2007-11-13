#include "Serial.cpp"

class Cabox : public Serial
{
public:
  
  bool init (const Params& p)
  {
    for(int i=0; i<12;i++) {
      mValues[i] = 0.0;
      mOffset[i] = 0.0;
    }
    
    mIndex = 0;
    mState = 0; // wait for sync
    mOffsetOnFull = true; // do not offset when 12 values are set
    mSlowPort = false;
    
    if (init_serial(p))
    {
      // enter read data
      return mPort.write_char('b');
    } else
      return false;
  }
  
  // outlet 1
  void receive(Signal& sig)
  {
    int c,r;
    double val;
    int i;
    sig.type = NilSignal; // most of the time we do not send
    bool read_count = 0;
    bool has_data = false;
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
      mValues[i] = val - mOffset[i];
      
      if (mIndex >= 24) {
        has_data = true;
        if (mOffsetOnFull) {
          for(int i=0; i<12; i++) {
            mOffset[i] = mValues[i] + mOffset[i];
          }
          mOffsetOnFull = false;
        }
        
        // write to buffer and find highest value/direction
        mHighestValue = 0;
        for(int i=0; i<12; i++) {
          float abs_val;
          val = mValues[i];
          write_buf(val);
          
          abs_val = val < 0 ? -val : val;
          // abs(..) casts to integer ????
          
          if ( abs_val > mHighestValue ) {
            mHighestDirection = i;
            mHighestValue = abs_val;
          }

        }
        
        
        mState = 0;
        mIndex = 0;
      }
    }
    if (has_data) {
      sig.type = FloatArraySignal;
      sig.floats.size  = mWindowSize * DEFAULT_WINDOW_COUNT;
      sig.floats.value = mBuffer + mReadPosition;
    } else {
      sig.set_nil();
    }
  }
  
  // send highest value
  void high(Signal& sig)
  {
    sig.set(mHighestValue);
  }
  
  // send highest value's direction
  void direction(Signal& sig)
  {
    sig.set(mHighestDirection);
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
private:
  double mValues[12];
  double mOffset[12];
  double mHighestValue;
  int mHighestDirection;
  int mIndex;
  int mState;
  int mHigh;
  bool mOffsetOnFull;
  bool mSlowPort;
};

extern "C" void init()
{
  CLASS (Cabox)
  OUTLET(Cabox,receive)
  OUTLET(Cabox,high)
  OUTLET(Cabox,direction)
  METHOD(Cabox,offset)
  SUPER_METHOD(Cabox, Script, set)
  SUPER_METHOD(Cabox, Script, load)
  SUPER_METHOD(Cabox, Script, script)
}