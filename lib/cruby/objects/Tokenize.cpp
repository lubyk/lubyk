#include "class.h"

/** Filtering modes. */
enum tokenize_filter_t {
  Peak          /**< Make a token out of peak index and value. */
};

#define TOK_VALUE_SCALE 4

/** Capture box feature extraction. Transforms time based signals into tokens. */
class Tokenize : public Node
{
public:
  
  bool init (const Params& p)
  {
    std::string mode = p.val("mode", std::string("peak"));
    if (mode == std::string("mode")) {
      mMode = Peak;
    } else {
      mMode = Peak;
    }
    mUnitSize  = p.val("unit", 1);
    mPeak      = p.val("peak", 1.0);
    mThreshold = p.val("threshold", mPeak * 0.2);
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    int label;
    if (sig.type != ArraySignal) {
      *mOutput << mName << ": wrong signal type '" << sig.type_name() << "' (should be ArraySignal)\n";
      return;
    } else if (sig.array.size % mUnitSize != 0) {
      *mOutput << mName << ": wrong signal dimension " << sig.array.size << ". Should be a multiple of " << mUnitSize << " (defined by 'unit' parameter).\n";
      return;
    }
    mWindowSize = sig.array.size / mUnitSize;
    mLiveStream = sig.array.value;
    
    switch(mMode) {
    case Peak:
      if (peak_tokenizer())
        send(mLabel);
      break;
    }
  }
  
private:
  bool peak_tokenizer()
  {
    int    max_id[2] = {0, 0};
    int    label;
    double max_value[2] = {0.0, 0.0};
    double value;
    double max_before_scale;
    
    // get two most important ids and peak values ( if peak value > 0, id is id [mUnitSize,2*mUnitSize -1], else [0,mUnitSize -1] )
    for(int i=0; i < mWindowSize * mUnitSize; i++) {
      value = mLiveStream[i];
      if (value < 0) value = - value;
      if (value > max_value[0]) {
        if (max_id[0] % mUnitSize != (i%mUnitSize)) {
          // store old 'second' maxima if it is not the same id as first.
          max_value[1] = max_value[0];
          max_id[1]    = max_id[0];
        }
        max_value[0] = value;
        max_id[0]    = (i % mUnitSize) + (mLiveStream[i] < 0 ? 0 : mUnitSize);
      } else if (value > max_value[1] && (i%mUnitSize) != max_id[0] % mUnitSize) {
        max_value[1] = value;
        max_id[1]    = (i % mUnitSize) + (mLiveStream[i] < 0 ? 0 : mUnitSize);
      }
    }
    if (mDebug)
      printf("%i:%.2f, %i:%.2f ==> ", max_id[0], max_value[0], max_id[1], max_value[1]);
    
    max_before_scale = max_value[0];
    /* reduce peak to an integer between [0,4] */
    if (max_value[0] > mPeak) max_value[0] = (TOK_VALUE_SCALE - 1);
    else max_value[0] = (int)((TOK_VALUE_SCALE - 1.0) * max_value[0] / mPeak); // floor
    
    if (max_value[1] > mPeak) max_value[1] = (TOK_VALUE_SCALE - 1);
    else max_value[1] = (int)((TOK_VALUE_SCALE - 1.0) * max_value[1] / mPeak); // floor
    
    label = (max_value[0] * 2 * mUnitSize * 2 * mUnitSize * TOK_VALUE_SCALE) + (max_id[0] * 2 * mUnitSize * TOK_VALUE_SCALE) + (max_value[1] * 2 * mUnitSize) + max_id[1];
    if (mDebug) {
      printf("%i:%.0f, %i:%.0f ==> ", max_id[0], max_value[0], max_id[1], max_value[1]);
      printf("%i\n", label);
    }
    if (max_before_scale > mThreshold && mLabel != labe) {
      mLabel = label;
      return true;
    } else
      return false;
  }
  
  
  tokenize_filter_t mMode; /**< Type of filtering to use. */
  int mLabel;              /**< Tokenization result. */
  int mUnitSize;           /**< How many values form a sampling unit (= number of signals). */
  int mWindowSize;         /**< Size of window (number of samples). */
  double mPeak;            /**< Peak value (used to resample). */
  double mThreshold;       /**< Only send if peak value is above threshold. */
  double * mLiveStream;    /**< Live data stream from buffer. */
};

extern "C" void init()
{
  CLASS(Tokenize)
  OUTLET(Tokenize, token)
}