#include "class.h"
#include "FFTReal.h"

class FFT : public Node
{
public:
  FFT() : mProcessor(NULL) {}
  
  virtual ~FFT()
  {
    if (mProcessor)   delete(mProcessor);
  }
  
  bool set (const Params& p)
  {
    size_t row_count = p.val("row", 64);
    size_t col_count = p.val("col", 1);
    return set_sizes(row_count, col_count);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    const Matrix * mat;
    if (!sig.get(&mat)) {
      *mOutput << mName << ": wrong signal type '" << sig.type_name() << "' (should be ArraySignal)\n";
      return;
    } else if (mat->row_count() != mFrequencies.row_count() || mat->col_count() != mFrequencies.col_count()) {
      if(!set_sizes(mat->row_count(), mat->col_count())) {
        *mOutput << mName << ": could not change matrix dimension to " << mat->row_count() << "x" << mat->col_count() << ".\n";
        return;
      }
    }
    
    size_t col_count = mFrequencies.col_count(); // mUnitSize
    size_t row_count = mFrequencies.row_count(); // sample_count
    if (col_count > 1) {
      for(size_t s=0; s < col_count; s++) {
        // compute FFT for each column (signal)
        
        // deinterlace the data...
        for(size_t i=0; i < row_count; i++) 
          mBuffer.data[s * col_count + i] = mat->data[s + col_count * i];
      
        // mBuffer.data (real) --- FFT ---> mFreqResult.data (complex)
        mProcessor->do_fft(mFreqResult.data, mBuffer.data + s*col_count);
        
        real_t real, img;
        for(size_t i=0; i < row_count/2; i++) {
          // interlace result back and compute power spectrum (real^2 + img^2)
          real =  mFreqResult.data[i];
          img  = -mFreqResult.data[row_count/2 + i];
          // polar coordinates (only show)
          /* */
          mFrequencies.data[s + col_count * i] = (real * real + img * img) / row_count; // divide by row_count to scale. FIXME: shouldn't it be sqrt(row_count) ?
          mFrequencies.data[s + col_count * (i + row_count/2)] = 0.0; // atan(b/a)
          /**/
          /* show only the norm accross all page
          norm = (real * real + img * img) / sample_count; // divide by sample_count to scale
          mFrequencies[s + mUnitSize * 2 * i] = norm;
          mFrequencies[s + mUnitSize * (2 * i + 1)] = norm;
          */
        
        }

      }  
      
    } else {
      // vector (real) --FFT---> mFrequencies (complex)
      mProcessor->do_fft(mFrequencies.data, mBuffer.data);
      // FIXME: polar coordinates for FFT on single signal
    }
    send(mS);
  }
  
  virtual void spy()
  {  
    bprint(mSpy, mSpySize,"");    
  }
  
private:
  
  bool set_sizes(size_t pRowCount, size_t pColCount)
  {
    if (mFrequencies.row_count() == pRowCount && mFrequencies.col_count() == pColCount) return true;
    
    // make sure it is a power of 2
    size_t size = 2;
    while (size < pRowCount) size *= 2;
    if (size != pRowCount) {
      *mOutput << mName << ": wrong signal dimension " << pRowCount << ". Should be a power of 2.\n";
      return false;
    }
    
    if (mProcessor) delete mProcessor;
    mProcessor = new FFTReal<real_t>(pRowCount);
    
    // allocate buffers
    
    if (!mFrequencies.set_sizes(pRowCount, pColCount)) return false;
    
    if (pColCount > 1) {
      if (!mFreqResult.set_sizes(1, pColCount))     return false; // temporary buffer
      if (!mBuffer.set_sizes(pRowCount, pColCount)) return false; // deinterlace buffer
    }
    
    mS.set(mFrequencies);
    return true;
  }
   
  FFTReal<real_t> * mProcessor; /**< FFT processor. */
  Matrix mFrequencies;          /**< Frequency output up to 1/2 of the signal, phase for other half. 
                                  *  Example for a 64 samples buffer:
                                  *  mFrequencies[0]      =  0
                                  *  mFrequencies[1 ..32] =  frequency amplitude of f[1..32]
                                  *  mFrequencies[33..63] = -imaginary part of f[33..63]
                                  *
                                  *  See 'fft/readme.txt' for details. */

  Matrix mBuffer;               /**< Deinterlace buffer used when more then one signal is present in the incoming
                                  *  stream (unit size > 1). */
  Matrix mFreqResult;           /**< Temporary buffer to store results before interlacing back when processing
                                  *  more then one signal at a time. */
};

extern "C" void init()
{
  CLASS(FFT)
  OUTLET(FFT,frequencies)
}