#include "Serial.cpp"

class Cabox : public Serial
{
public:
  
  bool init (const Params& p)
  {
    memset(mValues, 0, 12);
    mIndex = 0;
    mState = 0; // wait for sync
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
    sig.type = NilSignal; // most of the time we do not send
    
    if (!mPort.read_char(&c)) return;
            
    //printf("%i[%i]\n",mState,c);
    if (mState == 0) {
      if (c == 255) mState = 1;
      return;
    }
    
    if (mState == 1) { // first sync received
      if (c == 255) { // move into data mode
        mIndex = 0;
        mState = 2;
        return;
      } else
        mState = 0;
      return;
    }
    
    if (mState == 2 && ((mIndex % 2) == 0)) {
      mIndex += 1;
      mHigh = c;
      return;
    }
    
    val  = (mHigh * 256) + c;
    val  = (val/1024) * (3300 - 1650) / 200; // 3300 = fullscale_mv, 1024 = fullscale_bin, 1650 = offset_mv, 200 = sensitivity_mv
    mValues[(mIndex-1)/2] = val;
    mIndex += 1;
    if (mIndex >= 12) {
      mState = 0;
      mIndex = 0;
    }
printf("%.4f\n",mValues[0]);
    sig.set(mValues[0]);
  }
  
private:
  double mValues[12];
  int mIndex;
  int mState;
  int mHigh;
  /* data */
};

extern "C" void init()
{
  CLASS (Cabox)
  OUTLET(Cabox,receive)
}