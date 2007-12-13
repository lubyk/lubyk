#include "class.h"

class Average : public Node
{
public:
  bool set(const Params& p)
  {
    // nothing to set
    return true;
  }
  
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (sig.type == MatrixSignal) {
      const Matrix * mat;
      sig.get(&mat);
      
      if (!mat->row_count()) return;
      
      // copy first line of 'mat' into vector mMatrix (adapts mMatrix size)
      mMatrix.copy(*mat,0,0);
      
      // add every other row of 'mat' to vector mMatrix.
      for(size_t i=1; i < mat->row_count(); i++)
        mMatrix.add(*mat,i,i);
      
      
      mMatrix /= (double)mat->row_count();
      
      send(mMatrix);
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