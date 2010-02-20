#include "rubyk.h"
#include <iostream>

/** Filter char values. */
class MaxCount : public Node
{
public:
  bool set (const Value &p)
  {
    return true;
  }
  
  void bang(const Value &val)
  {
    if (val.type != MatrixValue) {
      *output_ << name_ << ": filter only works with array signals. Use a buffer.\n";
      return;
    }
    const Matrix * vector = val.matrix.value;
    size_t size = vector->col_count();
    
    for(int i=0; i < 256; i++) {
      mLabelVote[i] = 0;
    }
    
    int max_vote  = 0;
    int max_label = 0;
    for(size_t i=0; i < size; i++) {
      int label = ((unsigned int)vector->data[i]) % 256; // very simple vector quantization
      mLabelVote[label]++;
      if (mLabelVote[label] > max_vote) {
        max_vote  = mLabelVote[label];
        max_label = label;
      }
    }
    
    send(2, mLabelVote[max_label]);
    send(max_label);
  }
  
  virtual const Value inspect(const Value &val) 
  {  
    bprint(mSpy, mSpySize,"");    
  }

private:
  int mLabelVote[256];
};


extern "C" void init(Planet &planet) { 
  CLASS (MaxCount)
  OUTLET(MaxCount, value)
  OUTLET(MaxCount, votes)
}
