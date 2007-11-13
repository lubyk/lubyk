#include "class.h"

/** Svm states. */
enum svm_states_t {
  Waiting = 0,       /**< Initial state. Does nothing. This step is skipped for "Label" if possible. */
  ReadyToRecord,     /**< Ready to record new data. */
  CountDownPrepare,  /**< Start countdown "ready" */
  CountDownReady,    /**< Send events to say "ready" */
  CountDownSet,      /**< Send events to say "set"   */
  Recording,         /**< Send events to say "go!" */
  DataReceived,      /**< Send "done." */
  Validation,        /**< Waits for user confirmation (any char = save & record next, del = do not save), esc = exit. */
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
    mTempo      = p.val("tempo", 120);
    mBufferSize = mVectorSize * 1.25;
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
    
    enter(ReadyToRecord);
    return true;
  }

  void bang()
  {
    // do nothing, work done in 'command'
  }
  
  // inlet 1
  void command (const Signal& sig)
  {
    int cmd;
    if (!mIsOK) return; // no recovery
    
    if (!sig.get(&cmd)) return;
    
    switch(mState) {
    case Validation:
      if (cmd == 127) {
        // backspace ignore current vector
        enter(ReadyToRecord);
      } else if (cmd == '\n') {
        // save and wait
        store_data();
        enter(ReadyToRecord);
      } else {
        // any character: save and continue
        store_data();
        record_class(cmd);
      }
      break;
    case ReadyToRecord:
      if (cmd == '\n')
        try_to_label();
      else
        record_class(cmd);
    }
  }
  
  // inlet 2
  void data (const Signal& sig)
  {
    if (sig.type == DoubleArraySignal && sig.doubles.size >= mBufferSize) {
      mLiveBuffer = sig.doubles.value;
    } else {
      *mOutput << mName << ": invalid signal type (should be FloatArray with a minimum size of " << mBufferSize << ") " << sig << std::endl;
    }
  }

  // outlet 1
  void label(Signal& sig)
  {  
    if (!mIsOK) return; // no recovery
    
    switch(mState) {
    case Label:
      predict(sig);
      return;
    case DataReceived:
      receive_data();
      mState = Validation;
    }
  }

  /** Send 3,2,1,0 during countdown. */
  void countdown(Signal& sig)
  {
    switch(mState) {
    case CountDownPrepare:
      sig.set((int)2);
      mState = CountDownReady;
      bang_me_in(ONE_MINUTE / mTempo);
      break;
    case CountDownReady:
      sig.set((int)1);
      mState = CountDownSet;
      bang_me_in(ONE_MINUTE / mTempo);
      break;
    case CountDownSet:
      sig.set((int)0);
      bang_me_in((time_t)(ONE_SECOND * mVectorSize * 1.125 / mSampleRate)); // 12.5% margin at the end
      mState = Recording;
      break;
    case Recording:
      mState = DataReceived;
      sig.set((int)-1);
      break;
    }
  }
  
private:
  
  void predict(Signal& sig)
  {
    // TODO !
  }
  
  
  void try_to_label()
  {
    if (mReadyToLabel)
      mState = Label;
    else
      mState = Waiting;
  }
  
  void receive_data()
  {
    if (!mLiveBuffer) {
      *mOutput << mName << ": no data to record (nothing coming from inlet 2).\n";
      mBuffer = NULL;
      return;
    }
    
    // copy data into our local buffer
    memcpy(mBuffer, mLiveBuffer, mBufferSize * sizeof(double));
    
    // try to find the best bet by calculating minimal distance
    double distance, min_distance = 0.0;
    int   delta_used = mBufferSize - mVectorSize;
    for(int j = mBufferSize - mVectorSize; j >= 0; j--) {
      distance = 0.0;
      mVector = mBuffer + j;
      for(int i=0; i < mVectorSize; i++) {
        if (mVector[i] > mMeanVector[i])
          distance += mVector[i] - mMeanVector[i];
        else
          distance += mMeanVector[i] - mVector[i];
      }
      distance = distance / mVectorSize;
      if (distance < min_distance) {
        delta_used = j;
        min_distance = distance;
      }
    }
    mVector = mBuffer + delta_used;
    for(int i=0;i<12;i++)
      printf(" % .2f", mVector[i]);
    printf("\n");
    printf(": distance to mean vector %.3f (delta %i)\n~> Keep ? \n", distance, mBufferSize - mVectorSize - delta_used);
    //bprint(mBuf,mBufSize, ": distance to mean vector %.3f (delta %i)\n~> Keep ? ", distance, mBufferSize - mVectorSize - delta_used);
    //*mOutput << mName << mBuf << std::endl;
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
  
  void record_class(int cmd)
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
    mState = CountDownPrepare;
  }
  
  void enter(svm_states_t state)
  {
    mState = state;
    switch(mState) {
    case ReadyToRecord:
      *mOutput << mName << ": Ready to record\n~> ";
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
    //    3.1 read values into mVector
    //    3.2 execute function
    (this->*function)(mVector);
  }
  
  svm_states_t mState;
  bool mReadyToLabel; /**< Set to true when svm is up to date. */
  bool mReadyToLearn; /**< Set to true when there is data to learn from. */
  
  int mCountDown;
  double * mVector;     /**< Store a single vector. (points inside mBuffer)*/
  double * mMeanVector; /**< Store the mean value for all vectors from this class. */
  double * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  double * mBuffer;     /**< Store a single vector + 12.5% margin on both ends. */
  int mVectorSize;
  int mVectorCount;    /**< Number of vectors used to build the current mean value. */
  int mSampleRate; /**< How many samples per second do we receive from the 'data' inlet. */
  int mTempo;      /**< Countdown tempo. */
  int mBufferSize; /**< Size of buffered data ( = mVectorSize + 25%). We use more then the vector size to find the best fit. */
  int mClassLabel; /**< Current label. Used during recording and recognition. */
};


extern "C" void init()
{
  CLASS (Svm)
  INLET (Svm,command)
  INLET (Svm,data)
  OUTLET(Svm,label)
  OUTLET(Svm,countdown)
}