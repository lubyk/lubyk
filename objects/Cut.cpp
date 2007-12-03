#include "class.h"

class Cut : public Node
{
public:
  
  bool set (const Params& p)
  {
    mCutFrom = p.val("from", 1) ;
    mCutTo   = p.val("to", 32) ;
    if (mCutFrom > 0) mCutFrom--;
    if (mCutTo > 0) mCutTo--;
    mS.set(mCutMatrix);
    
    return true;
  }

  void bang(const Signal& sig)
  { 
    sig.get(&mLiveBuffer);
    
    if(mCutMatrix.set_view(*mLiveBuffer, mCutFrom, mCutTo)) {
      if (mDebug)
        *mOutput << mName << ": " << mS << std::endl;
      send(mS);
    } else {
      *mOutput << mName << ": " << mCutMatrix.error_msg() << std::endl;
    }
  }
  
private:
  const Matrix * mLiveBuffer;      /** Live input stream. */
  CutMatrix mCutMatrix;      /** Stores the cut context. */
  int      mCutFrom;     /** First value in output buffer. Counting from '1'. */
  int      mCutTo;       /** Last value in output buffer. Say from:1 to:5 to get [0,1,2,3,4].  */
};

extern "C" void init()
{
  CLASS (Cut)
  OUTLET(Cut, cut)
}