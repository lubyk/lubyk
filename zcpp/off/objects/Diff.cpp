#include "lubyk.h"

class Diff : public Node
{
public:
  bool init(const Value &p)
  {
    mDistance = 8;
    return true;
  }
  
  bool set(const Value &p)
  {
    p.get(&mDistance, "distance", true);
    return true;
  }
  
  // inlet 1
  void bang(const Value &val)
  {  
    const Matrix * live;
    if (val.type == MatrixValue) {
      if (!val.get(&live) || live->size() == 0) return;
      if (live->row_count() == 1) {
        // vector ==> value distance
        size_t col_count = live->col_count();
        if (col_count <= mDistance) {
          *output_ << name_ << ": vector too small (1x" << col_count << ") for distance (" << mDistance << ").\n";
          return;
        }
        send(live->data[live->size()-1] - live->data[live->size()-mDistance-1]);
      } else {
        // matrix ==> vector distance
        size_t row_count = live->row_count();
        if (row_count <= mDistance) {
          *output_ << name_ << ": not enough rows (" << row_count << "x" << live->col_count() << ") for distance (" << mDistance << ").\n";
          return;
        }
        TRY_RET(mBuffer, copy(*live, -1, -1)); // copy last row
        TRY_RET(mBuffer, subtract(*live, -mDistance-1, -mDistance-1));
        send(mBuffer);
      }
    } else {
      // pass through
      send(sig);
    }
  }
  
  virtual const Value inspect(const Value &val) 
  {
    bprint(mSpy, mSpySize,"%ix%i", mBuffer.col_count(), mBuffer.row_count());
  }
  
private:
  Matrix mBuffer;
  size_t mDistance;
};

extern "C" void init(Planet &planet) {
  CLASS(Diff)
  OUTLET(Diff,difference)
}