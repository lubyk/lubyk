#include "Serial.cpp"
#include "buffer.h"

class Cabox : public Serial
{
public:
  
  bool init (const Params& p)
  {
    size_t buffer_size = p.val("buffer", 1);
    if (buffer_size < 1) buffer_size = 1;
    
    mBuffer.set_sizes(buffer_size, 12);
    mOffset.set_sizes(1,12);
    
    mBuffer.clear();
    mOffset.clear();
    
    mHighestValue = 0.0;
    mHighestDirection = 0;
    mFindHighestValue = 0.0;
    mFindHighestDirection = 0;
    mVectorRateCounter = 0;
    mRate       = 0.0;
    mRateStart      = mServer->mCurrentTime;
    
    mIndex  = 0;
    mVector = mBuffer.advance();
    mState  = -3; // wait for sync
    mOffsetOnFull = true; // do not offset when 12 values are set
    
    if (init_serial(p))
    {
      // enter read data
      return mPort.write_char('b');
    } else
      return false;
  }
  
  bool set (const Params& p)
  {
    size_t buffer_size;
    if (p.get(&buffer_size, "buffer")) {
      if (buffer_size < 1) buffer_size = 1;
      mBuffer.set_sizes(buffer_size, 12);
      mBuffer.clear();
      mVector = mBuffer.advance();
      mState  = -3; // wait for sync
    }
    if (!set_serial(p)) return false;
    // enter read data
    return mPort.write_char('b');
  }
  
  
  void bang(const Signal& sig)
  {
    int c;
    double val;
    bool new_data = false;
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
      
      val -= mOffset.data[mIndex];
      mVector[mIndex] = val;
      //printf("val[%i] = %.2f  [%i,%i]\n",mIndex,val,mHigh,c);
      
      float abs_val;
      
      abs_val = val < 0 ? -val : val;
      // abs(..) casts to integer ????
      
      if ( abs_val > mFindHighestValue ) {
        mFindHighestDirection = mIndex;
        mFindHighestValue = abs_val;
      }
      
      mIndex++;
      if (mIndex >= 12) {
        mVectorRateCounter++;
        mState = -3; // wait for sync
        new_data = true;
        mHighestValue         = mFindHighestValue;
        mHighestDirection     = mFindHighestDirection;
        mFindHighestValue     = 0.0;
        mFindHighestDirection = 0;
        mBuffer.advance(); // move loop buffer forward
        if (mVectorRateCounter > 800) {
          mRate  = mVectorRateCounter * 1000.0 / (mServer->mCurrentTime - mRateStart);
          mVectorRateCounter = 0;
          mRateStart = mServer->mCurrentTime;
        }
        
        if (mOffsetOnFull) {
          mOffset.add(mVector, 12);
          mOffsetOnFull = false;
        }
      } else {
        mState--;
      }
    }
    
    if (new_data) {
      
      // outlet 3 (highest direction)
      send((int)mHighestDirection, 3);
      
      // outlet 2 (highest value)
      send(mHighestValue, 2);
      
      // outlet 1 (stream)
      mS.set(mBuffer.matrix());
      
      if (mDebug)
        *mOutput << mS << std::endl;
      
      send(mS);
    } 
  }
  
  void offset()
  {
    mOffsetOnFull = true;
  }
  
  void spy() 
  { bprint(mSpy, mSpySize,"%.2f /s", mRate );  }
  
private:
  
  Buffer   mBuffer; /**< Loop buffer to store the incoming values. */
  double * mVector; /**< Current vector to write data (points in mBuffer). */
  Matrix   mOffset;       /**< Vector with the offset for each signal. */
  double   mHighestValue;         /**< Last highest value of the signal. */
  size_t   mHighestDirection;     /**< Last highest direction of the signal. */
  double   mFindHighestValue;     /**< Current try at finding the highest value of the signal. */
  size_t   mFindHighestDirection; /**< Current try at finding the highest direction of the signal. */
  
  long int mVectorRateCounter; /**< Count number of vectors to measure data rate. */
  double   mRate;         /**< Data rate in vectors per second. */
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