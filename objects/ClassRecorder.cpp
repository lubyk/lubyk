
#include "class.h"
#include <errno.h>     // Error number definitions
#include <sys/types.h> // directory listing
#include <dirent.h>    // directory listing

/** Recorder states. */
enum class_recorder_states_t {
  ReadyToRecord,     /**< Ready to record new data. */
  CountDownReady,    /**< Send events to say "ready" */
  CountDownSet,      /**< Send events to say "set"   */
  Recording,         /**< Send events to say "go!" */
  Validation,        /**< Waits for user confirmation (any char = save & record next, del = do not save), esc = exit. */
};

class ClassRecorder : public Node
{
public:
  virtual ~ClassRecorder ()
  {
    if (mBuffer)     free(mBuffer);
    if (mMeanVector) free(mMeanVector);
  }

  bool init(const Params& p)
  {
    mVectorSize = p.val("vector", 32);
    mTempo      = p.val("tempo", 120);
    mUnitSize   = p.val("unit", 1);    // number of values form a sample
    mSampleRate = p.val("rate", 256);  // number of samples per second
    mMargin     = p.val("margin", 2.0);
    mFolder     = p.val("data", std::string("data"));
    mUseSnap    = p.val("snap", 1) == 1 ;
    
    mBufferSize = mVectorSize * (1.0 + mMargin);
    mLiveBuffer = NULL;
    mTrainFile  = NULL;
    mClassLabel = 0;
    
    mVectorCount = 0; // current mean value made of '0' vectors
    if (!alloc_doubles(&mMeanVector, mVectorSize, "mean vector")) return false;
    // clear mMeanVector
    for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    
    if (!alloc_doubles(&mBuffer, mBufferSize, "buffer")) return false;
    
    enter(ReadyToRecord);
    return true;
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    int cmd;
    
    if (!mIsOK) return; // no recovery
    
    if (sig.type == ArraySignal) {
      if (sig.array.size >= mBufferSize) {
        mLiveBuffer     = sig.array.value;
        mLiveBufferSize = sig.array.size;
      } else {
        *mOutput << mName << ": wrong signal size " << sig.array.size << " should be " << mBufferSize << " (with margin)\n.";
        return;
      }
    } else {
      time_t record_time = (time_t)(ONE_SECOND * mVectorSize)/(mUnitSize * mSampleRate);
      time_t record_with_margin = record_time;
      time_t countdown_time;
      if (record_time > 500)
        countdown_time = record_time;
      else
        countdown_time = 500;

      // receiving Bangs or command change
      sig.get(&cmd);
      
      switch(mState) {
      case CountDownReady:  
        bang_me_in(countdown_time);
        send_note(60 + (mClassLabel % 12),80,100,1,0,3);
        enter(CountDownSet);
        break;
      case CountDownSet:
        bang_me_in(record_with_margin); // 1/2 margin at the end
        send_note(72 + (mClassLabel % 12),80,record_time,1,0,3);
        enter(Recording);
        break;
      case Recording:
        receive_data();
        enter(Validation);
        break;
      case Validation:
        if (cmd == 127) {
          // backspace ignore current vector
          enter(ReadyToRecord);
          break;
        } else if (cmd == ' ') {
          // swap snap style
          if (mUseVectorOffset == mVectorOffset) {
            mUseVectorOffset = mBufferSize - mVectorSize * (1 + mMargin/2.0);
            *mOutput << mName << ": no-snap\n~> ";
          } else {
            mUseVectorOffset = mVectorOffset;
            *mOutput << mName << ": snap\n~> ";
          }
          break;
        } else if (cmd == RK_RIGHT_ARROW) { // -> right arrow 301
          mUseVectorOffset += mVectorSize;
          if (mUseVectorOffset > mBufferSize - mVectorSize) mUseVectorOffset = mBufferSize - mVectorSize;
          break;
        } else if (cmd == RK_LEFT_ARROW) { // <- left arrow  302
          mUseVectorOffset -= mVectorSize;
          if (mUseVectorOffset < 0) mUseVectorOffset = 0;
          break;
        } else {
          // any character: save and continue
          store_data();
        }
        // no break
      case ReadyToRecord:
        if (cmd == '\n') {
          enter(ReadyToRecord);
        } else {
          prepare_class_for_recording(cmd);
          enter(CountDownReady);
          bang_me_in(countdown_time);
          send_note(60 + (mClassLabel % 12),80,100,1,0,3);
        }  
        break;
      }
    }
    
    // send mean value
    if (mMeanVector) {
      mS.array.value = mMeanVector;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 2);
    }
    
    if (mState == Validation) {
      // recorded signal
      mS.array.value = mBuffer + mUseVectorOffset;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS);
    } else {
      // live signal
      mS.array.value = mLiveBuffer + mLiveBufferSize - mVectorSize;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS);
    }
    
  }
  
private:
  
  void receive_data()
  {
    double * vector;
    if (!mLiveBuffer) {
      *mOutput << mName << ": no data to record (no stream coming from inlet 1).\n";
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
        vector = mBuffer + j;
        for(int i=0; i < mVectorSize; i++) {
          d = (mMeanVector[i] - vector[i]) ;
          if (d > 0)
            distance += d;
          else
            distance -= d;
        }
        distance = distance / mVectorSize;
        if (min_distance < 0 || distance < min_distance) {
          delta_used = j;
          min_distance = distance;
        }
      }
      mVectorOffset = delta_used;
      bprint(mBuf,mBufSize, ": distance to mean vector %.3f (delta %i/%i)\nKeep ? ~> ", min_distance, delta_used, mBufferSize - mVectorSize - delta_used);
      *mOutput << mName << mBuf;
      fflush(stdout); // FIXME: should be related to *mOutput
    } else {
      mVectorOffset = mBufferSize - mVectorSize * (1 + mMargin/2.0);
      *mOutput << mName << ":~> Keep ? ";
      fflush(stdout); // FIXME: should be related to *mOutput
    }
    if (mUseSnap)
      mUseVectorOffset = mVectorOffset;
    else
      mUseVectorOffset = mBufferSize - mVectorSize * (1 + mMargin/2.0);
  }
  
  void store_data()
  {
    double * vector = mBuffer + mUseVectorOffset;
    
    if (!mBuffer) {
      *mOutput << mName << "(error): could not save data (empty buffer)\n~> ";
      return;
    }
    // 1. write to file
    FILE * file = fopen(mClassFile.c_str(), "ab");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << mClassFile << "' (" << strerror(errno) << ")\n~> ";
        return;
      }
      for(int i=0; i< mVectorSize; i++) {
        fprintf(file, " % .5f", vector[i]);
        if ((i+1)%mUnitSize == 0)
          fprintf(file, "\n");
      }  
      fprintf(file, "\n");  // two \n\n between vectors
    fclose(file);
    // 2. update mean value
    update_mean_value(vector);
  }
  
  void prepare_class_for_recording(int cmd)
  {
    // record character as class id
    if (mClassLabel != cmd)
    {
      // new class
      load_class(cmd, &ClassRecorder::update_mean_value);
    }
    *mOutput << mName << ": recording vector " << mVectorCount + 1 << " for " << cmd << "\n~> ";
  }
  
  void enter(class_recorder_states_t pState)
  {
    switch(pState) {
    case ReadyToRecord:
      mState = pState;
      *mOutput << mName << ": Ready to record\n~> ";
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
  void load_class(int cmd, void (ClassRecorder::*function)(double*))
  {
    mClassLabel = cmd;
    /** reset mean value. */
    mVectorCount = 0;
    for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    
    // 1. find file
    mClassFile = mFolder;
    bprint(mBuf, mBufSize, "/class_%i.txt", cmd);
    mClassFile.append(mBuf);
    
    // 2. open
    FILE * file;
    float val;
    int    value_count = 0;
    file = fopen(mClassFile.c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": new class\n";
        //*mOutput << mName << "(error): could not read from '" << mClassFile << "' (" << strerror(errno) << ")\n"
        return;
      }
      // read a vector
      while(fscanf(file, " %f", &val) != EOF) {
        fscanf(file, "\n"); // ignore newline
        mBuffer[value_count] = (double)val;
        if (value_count >= mVectorSize - 1) {
          // got one vector
          (this->*function)(mBuffer);
          value_count = 0;
        } else
          value_count++;
      }
    fclose(file);
  }
  
  class_recorder_states_t mState;
  std::string mFolder; /**< Folder containing the class data. */
  std::string mClassFile; /**< Current class data file. */
  FILE * mTrainFile;

  bool mUseSnap;      /**< True if the recording should snap to the best match. Usually better without. */
  
  int mCountDown;
  int mVectorOffset;     /**< Best match with this offset in mBuffer. */
  int mUseVectorOffset;  /**< Offset to use. */
  double * mMeanVector; /**< Store the mean value for all vectors from this class. */
  double * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  double * mBuffer;     /**< Store a single vector +  margin. */
  double   mMargin;     /**< Size (in %) of the margin. */
  int mVectorSize;
  int mUnitSize;       /**< How many values form a sample (single event). */
  int mVectorCount;    /**< Number of vectors used to build the current mean value. */
  int mTempo;          /**< Tempo for countdown and recording. */
  int mBufferSize;     /**< Size of buffered data ( = mVectorSize + 25%). We use more then the vector size to find the best fit. */
  int mLiveBufferSize;
  int mClassLabel; /**< Current label. Used during recording and recognition. */
  int mSampleRate; /**< Number of samples per second (used to compute recording time). */
};


extern "C" void init()
{
  CLASS (ClassRecorder)
  OUTLET(ClassRecorder,current)
  OUTLET(ClassRecorder,mean)
  OUTLET(ClassRecorder,countdown)
}