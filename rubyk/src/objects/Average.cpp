#include "rubyk.h"

class Average : public Node
{
public:
  bool set(const Value& p)
  {
    // nothing to set
    return true;
  }
  
  
  // inlet 1
  void bang(const Value& val)
  { 
    real_t d;
    const Matrix * live;
    if (sig.type == MatrixValue) {
      if (!sig.get(&live) || live->size() == 0) return;
      if (live->row_count() == 1) {
        // vector ==> value
        d = 0;
        for (size_t i=0; i < live->size(); i++)
          d += live->data[i];
        send(d / live->size());
      } else {
        // matrix ==> vector
        TRY_RET(mBuffer, copy(*live, 0, 0)); // copy first row
        for (size_t i=1; i < live->row_count(); i++) {
          TRY_RET(mBuffer, add(*live, i, i));
        }
        mBuffer /= live->row_count();
        send(mBuffer);
      }
    } else {
      // pass through
      send(sig);
    }
  }
  
  virtual const Value inspect(const Value& val) 
  {  
    bprint(mSpy, mSpySize,"%ix%i", mBuffer.col_count(), mBuffer.row_count());    
  }
private:
  Matrix mBuffer; /**< Average of incoming live stream. */
};

extern "C" void init()
{
  CLASS(Average)
  OUTLET(Average, average)
}