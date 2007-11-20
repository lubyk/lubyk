#include "class.h"


// could use code from http://ldesoras.free.fr/prod.html
class FFT : public Node
{
public:
  
  ~FFT()
  {
    if (mBufferX) free(mBufferX);
    if (mBufferY) free(mBufferY);
  }
  
  bool init (const Params& p)
  {
    mBufferSize = 0;
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (sig.type != ArraySignal) {
      *mOutput << mName << ": wrong signal type '" << sig.type_name() << "' (should be ArraySignal)\n";
      return;
    } else if (sig.array.size / 2 != 0) {
      *mOutput << mName << ": wrong signal dimension " << sig.array.size << ". Should be a power of 2.\n";
      return;
    }
    // copy buffer
    allocate_buffer(sig.array.size);
    memcpy(mBufferX, sig.array.value, mBufferSize);
    memset(mBufferY, 0, mBufferSize);
    
    in_place_fft(1,mBufferSize,mBufferX,mBufferY);
    map_to_amplitude_and_phase(mBufferSize, mBufferX, mBufferY);
  }
  
private:
  
  void allocate_buffer(int size)
  {
    if (mBufferSize == size) return;
    if (mBufferSize != size) {
      if (mBufferX) {
        mBufferX = (double*)realloc(mBufferX, size * sizeof(double));
      } else {
        mBufferX = (double*)malloc(size * sizeof(double));        
      }

      if (!mBufferX) {
        *mOutput << mName << ": could not allocate " << size << " doubles for buffer X.\n";
        return;
      }

      if (mBufferY) {
        mBufferY = (double*)realloc(mBufferY, size * sizeof(double));
      } else {
        mBufferY = (double*)malloc(size * sizeof(double));        
      }
      if (!mBufferY) {
        *mOutput << mName << ": could not allocate " << size << " doubles for buffer Y.\n";
        return;
      }
    }
    mBufferSize = size;
  }

  /*
     This computes an in-place complex-to-complex FFT 
     x and y are the real and imaginary arrays of 2^m points.
     dir =  1 gives forward transform
     dir = -1 gives reverse transform
     
     code from Paul Bourke (http://local.wasp.uwa.edu.au/~pbourke/other/dft/)
  */
  void in_place_fft(short int dir,long m,double *x,double *y)
  {
    long n,i,i1,j,k,i2,l,l1,l2;
    double c1,c2,tx,ty,t1,t2,u1,u2,z;
    
    /* Calculate the number of points */
    n = 1;
    for (i=0;i<m;i++) 
      n *= 2;
    
    /* Do the bit reversal */
    i2 = n >> 1;
    j = 0;
    for (i=0;i<n-1;i++) {
      if (i < j) {
        tx = x[i];
        ty = y[i];
        x[i] = x[j];
        y[i] = y[j];
        x[j] = tx;
        y[j] = ty;
      }
      k = i2;
      while (k <= j) {
        j -= k;
        k >>= 1;
      }
      j += k;
    }
    
    /* Compute the FFT */
    c1 = -1.0; 
    c2 = 0.0;
    l2 = 1;
    for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
        for (i=j;i<n;i+=l2) {
          i1 = i + l1;
          t1 = u1 * x[i1] - u2 * y[i1];
          t2 = u1 * y[i1] + u2 * x[i1];
          x[i1] = x[i] - t1; 
          y[i1] = y[i] - t2;
          x[i] += t1;
          y[i] += t2;
        }
        z =  u1 * c1 - u2 * c2;
        u2 = u1 * c2 + u2 * c1;
        u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
        c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
    }
    
    /* Scaling for forward transform */
    if (dir == 1) {
      for (i=0;i<n;i++) {
        x[i] /= n;
        y[i] /= n;
      }
    }
  }
   
  map_to_amplitude_and_phase(int size, double * vectorX, double * vectorY)
  {
    double amp, phase;
    for(int i=0; i < size; i++) {
      // 
    }
  }
  
  double * mBufferX; /**< Real component of signal.    (Frequency output) */
  double * mBufferY; /**< Complex component of signal. (Phase output)     */
  int      mBufferSize;
};

extern "C" void init()
{
  CLASS(FFT)
  OUTLET(FFT,phase)
}