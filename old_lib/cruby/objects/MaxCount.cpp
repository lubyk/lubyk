#include "class.h"
#include <iostream>

/** Filter char values. */
class MaxCount : public Node
{
public:
  bool init (const Params& p)
  {
    return true;
  }
  
  void bang(const Signal& sig)
  {
    if (sig.type != ArraySignal) {
      *mOutput << mName << ": filter only works with array signals. Use a buffer.\n";
      return;
    }
    double * vector = sig.array.value;
    int      size   = sig.array.size;
    
    for(int i=0; i < 256; i++) {
      mLabelVote[i] = 0;
    }
    
    int max_vote  = 0;
    int max_label = 0;
    for(int i=0; i < size; i++) {
      int label = ((unsigned int)vector[i]) % 256;
      mLabelVote[label]++;
      if (mLabelVote[label] > max_vote) {
        max_vote  = mLabelVote[label];
        max_label = label;
      }
    }
    if (mDebug) *mOutput << mName << ": " << max_label << ": " << mLabelVote[max_label] << std::endl;
    send(max_label);
  }

private:
  int mLabelVote[256];
};


extern "C" void init()
{ 
  CLASS (MaxCount)
  OUTLET(MaxCount, value)
}
