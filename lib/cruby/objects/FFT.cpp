#include "class.h"
#include "FFTReal.h"

class FFT : public Node
{
public:
  
  ~FFT()
  {
    if (mFrequencies) free  (mFrequencies);
    if (mInterlaceFreq) free(mInterlaceFreq);
    if (mBuffer)      free  (mBuffer);
    if (mProcessor)   delete(mProcessor);
  }
  
  bool init (const Params& p)
  {
    int buf_size = p.val("size", 64);
    mUnitSize    = p.val("unit", 1);
    
    // make sure it is a power of 2
    long size = 2;
    while (size < buf_size) size *= 2;
    if (size != buf_size) {
      *mOutput << mName << ": wrong signal dimension " << buf_size << ". Should be a power of 2.\n";
      mBufferSize = 0;
      return false;
    } else
      mBufferSize = buf_size * mUnitSize;
    
    mProcessor = new FFTReal<double>(buf_size);
    
    // allocate buffers
    
    mFrequencies = (double*)malloc(mBufferSize * sizeof(double));        
    if (!mFrequencies) {
      *mOutput << mName << ": could not allocate " << mBufferSize << " doubles for frequency result.\n";
      return false;
    }
    
    if (mUnitSize > 1) {
      mInterlaceFreq = (double*)malloc((mBufferSize/mUnitSize) * sizeof(double));        
      if (!mInterlaceFreq) {
        *mOutput << mName << ": could not allocate " << (mBufferSize/mUnitSize) << " doubles for temporary buffer.\n";
        return false;
      }
      
      mBuffer = (double*)malloc(mBufferSize * sizeof(double));        
      if (!mBuffer) {
        *mOutput << mName << ": could not allocate " << mBufferSize << " doubles for (deinterlace) buffer.\n";
        return false;
      }
    }
    
    mS.type = ArraySignal;
    mS.array.size  = mBufferSize;
    mS.array.value = mFrequencies;
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    if (sig.type != ArraySignal) {
      *mOutput << mName << ": wrong signal type '" << sig.type_name() << "' (should be ArraySignal)\n";
      return;
    } else if (sig.array.size != mBufferSize) {
      *mOutput << mName << ": wrong signal dimension " << sig.array.size << ". Should be " << mBufferSize << "\n";
      return;
    }
    
    if (mUnitSize > 1) {
      int sample_count = mBufferSize / mUnitSize;
      // should check data format first...
      for(int s=0; s < mUnitSize; s++) {
        for(int i=0; i < sample_count; i++) {
          // deinterlace the data...!!
          mBuffer[s * mUnitSize + i] = sig.array.value[s + mUnitSize * i];
        }
      
        // sig.array.value (real) --FFT---> mFrequencies (complex)
        mProcessor->do_fft(mInterlaceFreq, mBuffer + s*mUnitSize);
        int img_offset = sample_count/2;
        double real, img, norm;
        for(int i=0; i < img_offset; i++) {
          // interlace result back and compute power spectrum (real^2 + img^2)
          real = mInterlaceFreq[i];
          img  = -mInterlaceFreq[img_offset + i];
          // polar coordinates (only show)
          /* */
          mFrequencies[s + mUnitSize * i] = (real * real + img * img) / sample_count; // divide by sample_count to scale
          mFrequencies[s + mUnitSize * (i + img_offset)] = 0.0; // atan(b/a)
          /**/
          /* show only the norm accross all page
          norm = (real * real + img * img) / sample_count; // divide by sample_count to scale
          mFrequencies[s + mUnitSize * 2 * i] = norm;
          mFrequencies[s + mUnitSize * (2 * i + 1)] = norm;
          */
        }
      }  
      
    } else {
      // sig.array.value (real) --FFT---> mFrequencies (complex)
      mProcessor->do_fft(mFrequencies, sig.array.value);
    }
    
    send(mS);
  }
  
private:
   
  FFTReal<double> * mProcessor; /**< FFT processor. */
  double * mFrequencies;  /**< Frequency output up to 1/2 of the signal, phase for other half. 
                       *  Example for a 64 samples buffer:
                       *  mFrequencies[0]      =  0
                       *  mFrequencies[1 ..32] =  frequency amplitude of f[1..32]
                       *  mFrequencies[33..63] = -imaginary part of f[33..63]
                       *
                       *  See 'fft/readme.txt' for details. */
  double * mBuffer; /**< Deinterlace buffer used when more then one signal is present in the incoming stream (unit size > 1). */
  double * mInterlaceFreq; /**< Temporary buffer to store results before interlacing back when processing more then one signal at a time. */
  int      mBufferSize; /**< Size of the buffer for the output values. */
  int      mUnitSize; /**< How many different signals are present in the buffer. */
};

extern "C" void init()
{
  CLASS(FFT)
  OUTLET(FFT,phase)
}