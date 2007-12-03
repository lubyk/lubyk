#include "class.h"

class Average : public Node
{
public:
  Average() : mMatrix(NULL) {}
  
  ~Average() {}
  
  bool init (const Params& p)
  {  
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (sig.type == ArraySignal) {
      Matrix * mat = sig.array.value;
      
      size_t row_count = sig.array.value->row_count();
      if (!row_count) return;
      
      mMatrix.copy(mat,0,0);
      
      for(int i=1; i < row_count; i++)
          mMatrix.add(mat,i,i);
      
      mMatrix /= row_count;
      
      send(mMatrix);
      if (mDebug) *mOutput << mName << ": " << mMatrix << std::endl;
      
    }
  }
private:
  Matrix mMatrix; /**< Average of incoming live stream. */
};

extern "C" void init()
{
  CLASS(Average)
  OUTLET(Average, average)
}