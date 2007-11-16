#include "class.h"

/** Svm states. */
enum svm_states_t {
  ReadyToRecord,     /**< Ready to record new data. */
  CountDownReady,    /**< Send events to say "ready" */
  CountDownSet,      /**< Send events to say "set"   */
  Recording,         /**< Send events to say "go!" */
  Validation,        /**< Waits for user confirmation (any char = save & record next, del = do not save), esc = exit. */
  ValidationNoSnap,
  Learning,          /**< Thread launched to learn the new data. Only possible action is interrupt. */
  Label,             /**< Done learning or restored from file. Outputs labels. */
};

class Svm : public Node
{
public:
  virtual ~Svm ()
  {
    if (mBuffer)     free(mBuffer);
    if (mMeanVector) free(mMeanVector);
  }

  bool init(const Params& p)
  {
    mVectorSize = p.val("vector", 32);
    mSampleRate = p.val("rate", 256);
    mUnitSize   = p.val("unit", 1);    // number of values form a sample
    mMargin     = 1.0 + p.val("margin", 0.25);
    mBufferSize = mVectorSize * mMargin;
    mLiveBuffer = NULL;
    mClassLabel = 0;
    
    mVectorCount = 0; // current mean value made of '0' vectors
    mMeanVector = (double*)malloc(mVectorSize * sizeof(double));
    if (!mMeanVector) {
      *mOutput << mName << ": Could not allocate " << mVectorSize << " doubles for mean vector.\n";
      return false;
    } else {
      // clear mMeanVector
      for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    }
    
    mBuffer     = (double*)malloc(mBufferSize * sizeof(double));
    if (!mBuffer) {
      *mOutput << mName << ": Could not allocate " << mBufferSize << " doubles for buffer.\n";
      return false;
    }
    
    enter(Label);
    return true;
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    int cmd;
    int label;
    time_t record_time = (time_t)(ONE_SECOND * mVectorSize / (mSampleRate * mUnitSize));
    time_t countdown_time;
    if (record_time > 500)
      countdown_time = record_time;
    else
      countdown_time = 500;
    
    if (!mIsOK) return; // no recovery
    if (sig.type == ArraySignal && sig.array.size >= mBufferSize) {
      mLiveBuffer = sig.array.value;
      mLiveBufferSize = sig.array.size;
    } else {
      // receiving Bangs or command change
      sig.get(&cmd);

      switch(mState) {
      case ReadyToRecord:
        if (cmd == '\n')
          enter(Label);
        else {
          prepare_class_for_recording(cmd);
          enter(CountDownReady);
          bang_me_in(countdown_time);
          send((int)2, 2);
        }  
        break;
      case CountDownReady:  
        bang_me_in(countdown_time);
        send((int)1, 2);
        enter(CountDownSet);
        break;
      case CountDownSet:
        bang_me_in(record_time); // 1/2 margin at the end
        send((int)0, 2);
        enter(Recording);
        break;
      case Recording:
        send((int)-1, 2);
        receive_data();
        enter(Validation);
        break;
      case Validation:  
      case ValidationNoSnap:
        if (cmd == 127) {
          // backspace ignore current vector
          enter(ReadyToRecord);
        } else if (cmd == '\n') {
          // save and wait
          store_data();
          enter(ReadyToRecord);
        } else if (cmd == ' ') {
          // swap snap style
          if (mState == Validation) {
            mState = ValidationNoSnap;
            *mOutput << mName << ": no-snap\n";
          } else {
            mState = Validation;
            *mOutput << mName << ": snap\n";
          }
        } else {
          // any character: save and continue
          store_data();
          prepare_class_for_recording(cmd);
          enter(CountDownReady);
          bang_me_in(countdown_time);
          send((int)2, 2);
        }
        break;
    
      case Label:
        if (predict(&label)) {
          send(label);
        } else {
          send(gBangSignal);
        }
        return;
      }
    }
    
    // send mean value
    if (mMeanVector) {
      mS.array.value = mMeanVector;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 4);
    }
    
    // send current signal
    if (mState == Validation) {
      mS.array.value = mVector;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 3);
    } else if (mState == ValidationNoSnap) {
      mS.array.value = mBuffer + mBufferSize - mVectorSize;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 3);
    } else {
      mS.array.value = mLiveBuffer + mLiveBufferSize - mVectorSize;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 3);
    }
  }
  
  
private:
  
  bool predict(int * pLabel)
  {
    // TODO !
    return false;
  }
  
  
  void receive_data()
  {
    if (!mLiveBuffer) {
      *mOutput << mName << ": no data to record (nothing coming from inlet 2).\n";
      mBuffer = NULL;
      return;
    }
    
    // copy data into our local buffer
    memcpy(mBuffer, mLiveBuffer + mLiveBufferSize - mBufferSize, mBufferSize * sizeof(double));
    
    if (mVectorCount) {
      // try to find the best bet by calculating minimal distance
      double distance, min_distance = -1.0;
      double d;
      int   delta_used = mBufferSize - mVectorSize;
      for(int j = mBufferSize - mVectorSize; j >= 0; j -= mUnitSize) {
        distance = 0.0;
        mVector = mBuffer + j;
        for(int i=0; i < mVectorSize; i++) {
          d = (mMeanVector[i] - mVector[i]) ; //* mVector[i] * mMeanVector[i]; // more importance on big signals.
          if (d > 0)
            distance += d;
          else
            distance -= d;
        }
        distance = distance / mVectorSize;
        printf("D %i: %.3f\n", j, distance);
        if (min_distance < 0 || distance < min_distance) {
          delta_used = j;
          min_distance = distance;
        }
      }
      mVector = mBuffer + delta_used;
      bprint(mBuf,mBufSize, ": distance to mean vector %.3f (delta %i/%i)\n~> Keep ? ", min_distance, delta_used, mBufferSize - mVectorSize - delta_used);
      *mOutput << mName << mBuf << std::endl;
    } else {
      mVector = mBuffer + mBufferSize - mVectorSize;
      *mOutput << mName << ":~> Keep ? " << std::endl;
    }
    //printf(": distance to mean vector %.3f (delta %i)\n~> Keep ? \n", distance, mBufferSize - mVectorSize - delta_used);
  }
  
  void store_data()
  {
    if (!mBuffer) {
      *mOutput << mName << ": could not save data (empty buffer)\n";
      return;
    }
    // 1. write to file
    // TODO
    // 2. update mean value
    update_mean_value(mVector);
  }
  
  void prepare_class_for_recording(int cmd)
  {
    // record character as class id
    if (mClassLabel != cmd)
    {
      // new class
      load_class(cmd, &Svm::update_mean_value);
    }
    mClassLabel = cmd;
    if ((cmd <= 'z' && cmd >= 'a') || (cmd <= 'Z' && cmd >= 'A') || (cmd <= '9' && cmd >= '0'))
      *mOutput << mName << ": learning a new sample for '" << (char)cmd << "'\n";
    else
      *mOutput << mName << ": learning a new sample for " << cmd << "\n";
  }
  
  void enter(svm_states_t pState)
  {
    switch(pState) {
    case ReadyToRecord:
      mState = pState;
      *mOutput << mName << ": Ready to record\n~> ";
      break;
    case Label: 
      if (mReadyToLabel) {
        mState = pState;
      } else {
        *mOutput << mName << ": Cannot enter 'label' mode.\n";
        enter(ReadyToRecord);
      }
      break;
    default:
      mState = pState;
    }
  }
  
  /** Update the mean value with the current vector. */
  void update_mean_value(double* vector)
  {
    mVectorCount++;
    double map = (double)(mVectorCount - 1) / (double)(mVectorCount);
    for(int i=0; i < mVectorSize; i++) {
      mMeanVector[i] = (mMeanVector[i] * map) + ( vector[i] / (double)mVectorCount );
    }
  }
  
  /** Execute the function for each vector contained in the class. */
  void load_class(int cmd, void (Svm::*function)(double*))
  {
    /** reset mean value. */
    mVectorCount = 0;
    for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    
    // 1. find file
    // 2. open
    // 3. for each vector
    //    3.1 read values into mBuffer
    //    3.2 execute function
    // (this->*function)(mBuffer);
  }
  
  svm_states_t mState;
  bool mReadyToLabel; /**< Set to true when svm is up to date. */
  bool mReadyToLearn; /**< Set to true when there is data to learn from. */
  
  int mCountDown;
  double * mVector;     /**< Store a single vector. (points inside mBuffer)*/
  double * mMeanVector; /**< Store the mean value for all vectors from this class. */
  double * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  double * mBuffer;     /**< Store a single vector +  margin. */
  double   mMargin;     /**< Size (in %) of the margin. */
  int mVectorSize;
  int mUnitSize;       /**< How many values form a sample (single event). */
  int mVectorCount;    /**< Number of vectors used to build the current mean value. */
  int mSampleRate; /**< How many samples per second do we receive from the 'data' inlet. */
  int mBufferSize; /**< Size of buffered data ( = mVectorSize + 25%). We use more then the vector size to find the best fit. */
  int mLiveBufferSize;
  int mClassLabel; /**< Current label. Used during recording and recognition. */
};


extern "C" void init()
{
  CLASS (Svm)
  OUTLET(Svm,label)
  OUTLET(Svm,countdown)
  OUTLET(Svm,current)
  OUTLET(Svm,mean)
}