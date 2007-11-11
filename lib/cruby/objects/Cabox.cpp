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
    mOffsetOnFull = false; // do not offset when 12 values are set
    mSlowPort = false;
    
    if (serial_init(p))
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
    
      if (mIndex == 24)
        printf("%.4f\n",mValues[1]);
      
      if (mIndex >= 24) {
        if (mOffsetOnFull){
          for(int i=0; i<12;i++) {
            mOffset[i] = mValues[i] + mOffset[i];
          }
          mOffsetOnFull = false;
        }
        mState = 0;
        mIndex = 0;
      }
    }
    sig.set(mValues[1]);
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
private:
  double mValues[12];
  double mOffset[12];
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
  METHOD(Cabox,offset)
}