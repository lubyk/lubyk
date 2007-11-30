#include "class.h"
#include "FFTReal.h"

class FFT : public Node
{
public:
  
  ~FFT()
  {
    if (mFrequencies) free  (mFrequencies);
    if (mFreqResult) free(mFreqResult);
    if (mBuffer)      free  (mBuffer);
    if (mProcessor)   delete(mProcessor);
  }
  
  bool init (const Params& p)
  {
    int buf_size  = p.val("size", 64);
    mUnitSize     = p.val("unit", 1);
    mShortTimeFFT = (p.val("stft", 0) == 1); /**< Do a short-time FFT (output peak frequency for each time t). 
                                               * This takes too much CPU, it is better to do :
                                               * small buffer -> fft -> buffer. */
    
    //if (mWindowSize % 2 == 0) mWindowSize += 1; /**< To make sure we have a 1.0 value in the center of the Hamming window. */
    
    
    
    // make sure it is a power of 2
    long size = 2;
    while (size < buf_size) size *= 2;
    if (size != buf_size) {
      *mOutput << mName << ": wrong signal dimension " << buf_size << ". Should be a power of 2.\n";
      mBufferSize = 0;
      return false;
    } else
      mBufferSize = buf_size * mUnitSize;
    
    mWindowSize = buf_size / 4; /**< Must also be a power of 2. */
    
    if (mShortTimeFFT)
      mProcessor = new FFTReal<double>(mWindowSize);
    else
      mProcessor = new FFTReal<double>(buf_size);
    
    // allocate buffers
    
    if (mShortTimeFFT) {

      if (!alloc_doubles(&mWindow, mWindowSize, "STFT window")) return false;

      if (!alloc_doubles(&mWindowedBuffer, mWindowSize, "STFT windowed buffer")) return false;
      
      // compute Hamming window
      for(int i=0; i < mWindowSize; i++) {
        mWindow[i] = 0.53836 - 0.46164 * cos(2 * PI * i / (mWindowSize - 1));
      }
    }
    
    
    if (!alloc_doubles(&mFrequencies, mBufferSize, "frequency result")) return false;
    
    if (mUnitSize > 1) {
      if (!alloc_doubles(&mFreqResult, mBufferSize/mUnitSize, "temporary frequency result")) return false;
      
      if (!alloc_doubles(&mBuffer, mBufferSize, "deinterlace buffer")) return false;
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
    
    double * vector = sig.array.value;
    
    if (mUnitSize > 1) {
      int sample_count = mBufferSize / mUnitSize;
      // should check data format first...
      for(int s=0; s < mUnitSize; s++) {
        if (mShortTimeFFT) {
          // mWindow: o o x o
          for(int w=0; w < sample_count; w++) { // FIXME: this increment is too small
            // move window along the samples
            int i_min, i_max;
            i_min = w - mWindowSize / 2;
            if (i_min < 0) i_min = 0;
            i_max = w + mWindowSize / 2;
            if (i_max > sample_count) i_max = sample_count;
            
            int j = 0;
            for(int i=i_min; i < i_max; i++) {
              // compute windowedBuffer for this region
              mWindowedBuffer[j] = vector[s + mUnitSize * i] * mWindow[j];
              j++;
            }
            
            // vector (real) --FFT---> mFrequencies (complex)
            mProcessor->do_fft(mFreqResult, mWindowedBuffer);
            
            int peak_id;
            double peak_value, real, img, norm;
            peak_value = 0;
            for(int i=0; i < mWindowSize/2; i++) {
              real = mFreqResult[i];
              img  = -mFreqResult[mWindowSize/2 + i];
              norm = (real * real + img * img) / mWindowSize;
              if(norm > peak_value) {
                peak_value = norm;
                peak_id    = i;
              }
            }
            if (peak_value > 0.001) // FIXME: this limit should be better defined
              mFrequencies[s + mUnitSize * w] = 8 * (double)peak_id / (double)mWindowSize; // peak frequency
            else
              mFrequencies[s + mUnitSize * w] = 0.0;
          }
        } else {
          // not short-time FFT
          for(int i=0; i < sample_count; i++) {
            // deinterlace the data...!!
            mBuffer[s * mUnitSize + i] = vector[s + mUnitSize * i];
          }
        
          // vector (real) --FFT---> mFrequencies (complex)
          mProcessor->do_fft(mFreqResult, mBuffer + s*mUnitSize);
          
          double real, img;
          for(int i=0; i < sample_count/2; i++) {
            // interlace result back and compute power spectrum (real^2 + img^2)
            real = mFreqResult[i];
            img  = -mFreqResult[sample_count/2 + i];
            // polar coordinates (only show)
            /* */
            mFrequencies[s + mUnitSize * i] = (real * real + img * img) / sample_count; // divide by sample_count to scale. FIXME: shouldn't it be sqrt(sample_count) ?
            mFrequencies[s + mUnitSize * (i + sample_count/2)] = 0.0; // atan(b/a)
            /**/
            /* show only the norm accross all page
            norm = (real * real + img * img) / sample_count; // divide by sample_count to scale
            mFrequencies[s + mUnitSize * 2 * i] = norm;
            mFrequencies[s + mUnitSize * (2 * i + 1)] = norm;
            */
          
          }
        }
      }  
      
    } else {
      // FIXME: STFT on single stream
      
      // vector (real) --FFT---> mFrequencies (complex)
      mProcessor->do_fft(mFrequencies, vector);
    }
    if (mDebug)
      *mOutput << mName << ": " << mS << std::endl; // FIXME: send oldest values not most recent
    send(mS);
  }
  
private:
   
  FFTReal<double> * mProcessor; /**< FFT processor. */
  double *        mFrequencies; /**< Frequency output up to 1/2 of the signal, phase for other half. 
                                  *  Example for a 64 samples buffer:
                                  *  mFrequencies[0]      =  0
                                  *  mFrequencies[1 ..32] =  frequency amplitude of f[1..32]
                                  *  mFrequencies[33..63] = -imaginary part of f[33..63]
                                  *
                                  *  See 'fft/readme.txt' for details. */
  bool     mShortTimeFFT;       /**< Execute a short-time Fast Fourier Transform. */
  double * mWindowedBuffer;     /**< Used to compute the product x(n) * w(n-m) during STFT. */
  double * mWindow;             /**< Pre-computed window for STFT. (Using Hamming window). */
  int      mWindowSize;         /**< Size of the window to use during STFT. */
  double * mBuffer;             /**< Deinterlace buffer used when more then one signal is present in the incoming
                                  *  stream (unit size > 1). */
  double * mFreqResult;         /**< Temporary buffer to store results before interlacing back when processing
                                  *  more then one signal at a time. */
  int      mBufferSize;         /**< Size of the buffer for the output values. */
  int      mUnitSize; /**< How many different signals are present in the buffer. */
};

extern "C" void init()
{
  CLASS(FFT)
  OUTLET(FFT,phase)
}