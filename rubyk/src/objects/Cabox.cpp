#include "Serial.cpp"
#include "buffer.h"

class Cabox : public Serial
{
public:
  
  bool init (const Params& p)
  { 
    mBuffer.set_sizes(1, 12);
    mOffset.set_sizes(1, 12);
    
    mBuffer.clear();
    mOffset.clear();
    
    mHighestValue         = 0.0;
    mHighestDirection     = 0;
    mFindHighestValue     = 0.0;
    mFindHighestDirection = 0;
    mVectorRateCounter    = 0;
    mRate                 = 0.0;
    mRateStart            = mServer->mCurrentTime;
    

    mIndex  = 0;
    mVector = mBuffer.advance();
    mState  = -3; // wait for sync
    
    REGISTER_EVENT(Cabox, initial_offset, 200, NULL);
    
    // we accept serial init failures to use 'pseudo serial' with 'bang'
    init_serial(p);
    return true;
  }
  
  bool set (const Params& p)
  {
    size_t buffer_size;
    if (p.get(&buffer_size, "buffer")) {
      if (buffer_size < 1) buffer_size = 1;
      mBuffer.set_sizes(buffer_size, 12);
      mBuffer.clear();
      mVector = mBuffer.advance();
      if (!mVector) return false;
      mState  = -3; // wait for sync
    }
    // we accept serial init failures to use 'pseudo serial' with 'bang'
    set_serial(p);

    return true;
  }
  
  
  void bang(const Signal& sig)
  {
    if (!mIsOK) return;
    
    int c;
    int sig_i, sig_max = 0;
    real_t val;
    real_t * sig_buf = NULL;
    real_t sig_val;
    bool new_data = false;
    
    if (sig.type == MatrixSignal) {
      sig_max = sig.matrix.value->size();
      sig_i   = 0;
      sig_buf = sig.matrix.value->data;
    } else if (sig.get(&sig_val)) {
      sig_max = 1;
      sig_i   = 0;
      sig_buf = &sig_val;
    }
    //printf("%p %i %i %i\n", sig_buf, sig_i, sig_max, (int)sig_buf[sig_i]);
    while (mPort.read_char(&c) || (sig_buf && sig_i < sig_max && (c=(int)sig_buf[sig_i++]) >= 0 )) {
      // empty buffer (readall)
      //printf("%i:%i (%i)\n", mState, c, mIndex);
      
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
      
      val -= mOffset.data[mIndex];
      mVector[mIndex] = val;
      
      //printf("val[%i] = %.2f  [%i,%i]\n",mIndex,val,mHigh,c);
      
      float abs_val;
      
      abs_val = val < 0 ? -val : val;
      // abs(..) casts to integer ????
      
      if ( abs_val > mFindHighestValue ) {
        mFindHighestDirection = val < 0 ? mIndex + 6 : mIndex;
        mFindHighestValue = abs_val;
      }
      
      mIndex++;
      if (mIndex >= 12) {
        mVectorRateCounter++;
        mIndex = 0;
        mState = -3; // wait for sync
        new_data = true;
        
        if (mOffsetOnFull) {
          mOffset.add(mVector, 12);
          mOffsetOnFull = false;
        }
        
        mHighestValue         = mFindHighestValue;
        mHighestDirection     = mFindHighestDirection;
        mFindHighestValue     = 0.0;
        mFindHighestDirection = 0;
        mVector = mBuffer.advance(); // move loop buffer forward
        if (mVectorRateCounter > 800) {
          mRate  = mVectorRateCounter * 1000.0 / (mServer->mCurrentTime - mRateStart);
          mVectorRateCounter = 0;
          mRateStart = mServer->mCurrentTime;
        }
        
      } else {
        mState--;
      }
    }
    
    if (new_data) {
      
      // outlet 3 (highest direction)
      send(3, (int)mHighestDirection);
      
      // outlet 2 (highest value)
      send(2, mHighestValue);
      
      // outlet 1 (stream)
      mS.set(mBuffer.matrix());
      
      send(mS);
    } 
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
  void initial_offset(void * data)
  {
    mOffsetOnFull = true;
  }
  
  void spy() 
  { bprint(mSpy, mSpySize,"%.2f /s", mRate );  }
  
private:
  
  Buffer   mBuffer; /**< Loop buffer to store the incoming values. */
  real_t * mVector; /**< Current vector to write data (points in mBuffer). */
  Matrix   mOffset;       /**< Vector with the offset for each signal. */
  real_t   mHighestValue;         /**< Last highest value of the signal. */
  size_t   mHighestDirection;     /**< Last highest direction of the signal. */
  real_t   mFindHighestValue;     /**< Current try at finding the highest value of the signal. */
  size_t   mFindHighestDirection; /**< Current try at finding the highest direction of the signal. */
  
  long int mVectorRateCounter; /**< Count number of vectors to measure data rate. */
  real_t   mRate;         /**< Data rate in vectors per second. */
  time_t   mRateStart;    /**< Used to record time intervals between values to compute data rate. */
  size_t   mIndex;        /**< Current vector's value index (next write position). */
  int      mState;        /**< Synchronization flag. */
  int      mHigh;         /**< High byte. */
  bool     mOffsetOnFull; /**< If 'true', the next time we have a full vector, reset offset with these values. */
};

extern "C" void init()
{
  CLASS (Cabox)
  OUTLET(Cabox,stream)
  OUTLET(Cabox,high)
  OUTLET(Cabox,direction)
  METHOD(Cabox,offset)
  SUPER_METHOD(Cabox, Script, load)
  SUPER_METHOD(Cabox, Script, script)
}