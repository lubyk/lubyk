#include "rubyk.h"

class Cut : public Node
{
public:
  
  bool set (const Value &p)
  {
    mCutFrom = p.val("from", 1) ;
    mCutTo   = p.val("to", -1 ) ;
    mFlatten = p.val("flat", false);
    if (mCutFrom > 0) mCutFrom--;
    if (mCutTo > 0) mCutTo--;
    mS.set(mCutMatrix);
    
    return true;
  }

  void bang(const Value &val)
  { 
    val.get(&mLiveBuffer);
    
    TRY_RET(mCutMatrix, set_view(*mLiveBuffer, mCutFrom, mCutTo));
    if (mFlatten) mCutMatrix.set_sizes(1, mCutMatrix.size());
    send(mS);
  }
  
  void spy() 
  { bprint(mSpy, mSpySize, "[%i,%i]", mCutFrom < 0 ? mCutFrom : mCutFrom + 1, mCutTo < 0 ? mCutTo : mCutTo + 1); }
  
private:
  const Matrix * mLiveBuffer;      /** Live input stream. */
  CutMatrix mCutMatrix;      /** Stores the cut context. */
  int      mCutFrom;     /** First value in output buffer. Counting from '1'. */
  int      mCutTo;       /** Last value in output buffer. Say from:1 to:5 to get [0,1,2,3,4].  */
  bool     mFlatten;     /**< Transform matrix into vector. */
};

extern "C" void init(Planet &planet) {
  CLASS (Cut)
  OUTLET(Cut, cut)
}