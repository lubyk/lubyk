
#include "rubyk.h"
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
  bool init(const Value& p)
  {
    // defaults
    mTempo       = 120;
    mSampleRate  = 256;
    mMargin      = 1.5;
    mFolder      = "data";
    mUseSnap     = true;
    mClassLabel  = 0;
    mChannel     = 1;
    mVectorCount = 0;
    mTrainFile   = NULL;
    mMeanValue.set(mMeanVector);
    
    mMeanValue.set_meta(H("sample_offset"), 0); // shift display window right / left
    mMeanValue.set_meta(H("sample_count"),  0); // total number of samples per window when computing width_ratio
    mMeanValue.set_meta(H("draw_box"), true);   // draw a surrounding box
    
    mLiveValue.set(mLiveView);
    mBufferValue.set(mBuffer);
    mS.set(mBuffer);

    if (!resize(4,8)) return false;
    
    enter(ReadyToRecord);
    return true;
  }

  bool set(const Value& p)
  {
    // vector sizes is set from incomming stream
    p.get(&mTempo     ,"tempo");
    p.get(&mSampleRate,"rate");
    p.get(&mMargin    ,"margin");
    p.get(&mFolder    ,"data");
    p.get(&mUseSnap   ,"snap");
    p.get(&mChannel   ,"channel");
    
    return true;
  }

  // inlet 1
  void bang (const Value val)
  {
    int cmd;
    
    if (!mIsOK) return; // no recovery
    
    if (val.type == MatrixValue) {
      if (!resize(val.matrix.value->row_count(), val.matrix.value->col_count())) return;
      val.get(&mLiveBuffer);
      if (mLiveBuffer) {
        TRY_RET(mLiveView, set_view(*mLiveBuffer, 0, -1));
      }
      mHasLiveData = true;
    } else {
      time_t record_time = (time_t)(ONE_SECOND * mMeanVector.row_count())/(mSampleRate);
      time_t record_with_margin = record_time * (1 + mMargin);
      time_t countdown_time;
      if (mTempo > 0)
        countdown_time = 60000.0 / mTempo;
      else
        countdown_time = 0;

      // receiving Bangs or command change
      val.get(&cmd);
      
      switch(mState) {
      case CountDownReady:
        if (mTempo == 0) {
          // record directly
          bang_me_in(record_with_margin);
          send_note(3, 72 + (mClassLabel % 12),80,record_time,mChannel);
          enter(Recording);
        } else {
          bang_me_in(countdown_time);
          send_note(3, 60 + (mClassLabel % 12),80,100,mChannel);
          enter(CountDownSet);
        }
        break;
      case CountDownSet:
        bang_me_in(record_with_margin); // 1/2 margin at the end
        send_note(3, 72 + (mClassLabel % 12),80,record_time,mChannel);
        enter(Recording);
        break;
      case Recording:
        receive_data();
        enter(Validation);
        send(2, mMeanValue);    // reference signal
        send(mBufferValue);       // recorded signal
        break;
      case Validation:
        if (cmd == 127) {
          // backspace ignore current vector
          enter(ReadyToRecord);
          break;
        } else if (cmd == ' ') {
          // swap snap style
          if (mRowOffset == mSnapOffset) {
            mRowOffset = mRowMargin;
            *mOutput << mName << ": no-snap\n~> ";
          } else {
            mRowOffset = mSnapOffset;
            *mOutput << mName << ": snap\n~> ";
          }
          
          mMeanValue.set_meta(H("sample_offset"), mRowOffset);
          send(mBufferValue);       // recorded signal
          break;
        } else if (cmd == RK_RIGHT_ARROW) { // -> right arrow  301
          mRowOffset++;
          if (mRowOffset >= mBuffer.row_count() - mMeanVector.row_count())
            mRowOffset = mBuffer.row_count() - mMeanVector.row_count() - 1;
          
          mMeanValue.set_meta(H("sample_offset"), mRowOffset);
          send(mBufferValue);       // recorded signal
          break;
        } else if (cmd == RK_LEFT_ARROW) { // <- left arrow 302
          mRowOffset --;
          if (mRowOffset < 0)
            mRowOffset = 0;
          
          mMeanValue.set_meta(H("sample_offset"), mRowOffset);
          send(mBufferValue);       // recorded signal
          break;
        } else {
          // any character: save and continue
          store_data();
        }
        // no break
      case ReadyToRecord:
        mRowOffset = mRowMargin;
        mMeanValue.set_meta(H("sample_offset"), mRowOffset);
        
        if (cmd == '\n') {
          enter(ReadyToRecord);
        } else if (cmd == 127) {
          // clear mean vector
          mClassLabel  = 0;
          mVectorCount = 0;
          mMeanVector.clear();
          enter(ReadyToRecord);
        } else {
          prepare_class_for_recording(cmd);
          enter(CountDownReady);
          bang_me_in(countdown_time);
          send_note(3, 60 + (mClassLabel % 12),80,100,mChannel);
        }  
        break;
      }
    }

    // send mean value
    
    if (mState != Validation) {
      send(2, mMeanValue);
      send(mLiveValue); // live signal
    }
  }
  
  virtual const Value inspect(const Value val) 
  {  
    bprint(mSpy, mSpySize,"%s", mFolder.c_str());    
  }
  
private:
  
  void receive_data()
  {
    if (!mHasLiveData) {
      *mOutput << mName << ": no data to record (no stream coming from inlet 1).\n";
      return;
    }
    
    // copy data into our local buffer
    TRY_RET(mBuffer, copy(*mLiveBuffer));
    
    if (mRowMargin) {
      // try to find the best bet by calculating minimal distance
      real_t distance, min_distance = -1.0;
      real_t d;
      real_t * vector;
      real_t * mean = mMeanVector.data;
      int   delta_used = mBuffer.row_count() - (int)mMeanVector.row_count();
      for(int j = (int)mBuffer.row_count() - (int)mMeanVector.row_count(); j >= 0; j--) {
        distance = 0.0;
        vector = mBuffer[j];
        for(size_t i=0; i < mMeanVector.size(); i++) {
          d = (mean[i] - vector[i]) ;
          if (d > 0)
            distance += d;
          else
            distance -= d;
        }
        distance = distance / mMeanVector.size();
        if (min_distance < 0 || distance < min_distance) {
          delta_used = j;
          min_distance = distance;
        }
      }
      mSnapOffset = delta_used;
      *mOutput << mName << ": distance to mean vector " << min_distance << " (delta " << delta_used << "/" << mBuffer.row_count() - mMeanVector.row_count() << ")\nKeep ? ~> ";
      fflush(stdout); // FIXME: should be related to *mOutput
    } else {
      mSnapOffset = mBuffer.row_count() - mMeanVector.row_count();
      *mOutput << mName << ":~> Keep ? ";
      fflush(stdout); // FIXME: should be related to *mOutput
    }
    
    if (mUseSnap && mVectorCount > 0) {
      mRowOffset = mSnapOffset;
    } else {
      mRowOffset = mRowMargin;
    }
    
    mMeanValue.set_meta(H("sample_offset"), mRowOffset);
  }
  
  void store_data()
  {
    // 0. set view
    TRY_RET(mView,     set_view(mBuffer, mRowOffset, mRowOffset + mMeanVector.row_count() - 1));
    
    // 1. write to file
    if (!mView.to_file(mClassFile, "ab")) {
      *mOutput << mName << ": could not write vector (" << mView.error_msg() << ")\n";
    }
    
    // 2. update mean value
    update_mean_value(mView);
  }
  
  void prepare_class_for_recording(int cmd)
  {
    // record character as class id
    if (mClassLabel != cmd)
    {
      // new class
      load_class(cmd, &ClassRecorder::update_mean_value);
    }
    *mOutput << mName << ": recording vector " << mVectorCount + 1 << " for " << (char)cmd << "\n~> ";
  }
  
  void enter(class_recorder_states_t pState)
  {
    switch(pState) {
    case ReadyToRecord:
      *mOutput << mName << ": Ready to record\n~> ";
      break;
    case Recording:
      mHasLiveData = false;
      break;
    default:
      ;// do nothing
    }
    mState = pState;
  }
  
  /** Update the mean value with the current vector. */
  void update_mean_value(const Matrix& pVector)
  {
    mVectorCount++;
    real_t map = (real_t)(mVectorCount - 1) / (real_t)(mVectorCount);  // avg = (avg * (n-1)/n) + value/n
    mMeanVector *= map;
    mMeanVector.add(pVector, 0, -1, 1.0 / mVectorCount);
  }
  
  /** Execute the function for each vector contained in the class. */
  void load_class(int cmd, void (ClassRecorder::*function)(const Matrix& pVector))
  {
    Matrix vector;
    
    TRY_RET(vector, set_sizes(mMeanVector.row_count(), mMeanVector.col_count()));
    
    mClassLabel = cmd;
    /** reset mean value. */
    mVectorCount = 0;
    mMeanVector.clear();
    
    // 1. find file
    mClassFile = mFolder;
    std::string str;
    bprint(str,"/class_%c.txt", cmd);
    mClassFile.append(str);
    
    // 2. open
    FILE * file = fopen(mClassFile.c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": new class\n";
        return;
      }
      // read a vector
      while(vector.from_file(file)) (this->*function)(vector);
    
    fclose(file);  
    mMeanValue.set_meta(H("sample_offset"), mRowMargin);
  }
  
  // resize internal buffers from incomming stream
  bool resize(size_t pRowCount, size_t pColCount)
  {
    if (mBuffer.row_count() != pRowCount || mBuffer.col_count() != pColCount) {
      size_t vector_size = pRowCount / (1.0 + (2.0 * mMargin));
      mRowMargin = (pRowCount - vector_size) / 2;
      
      TRY(mBuffer,     set_sizes(pRowCount,   pColCount));
      TRY(mMeanVector, set_sizes(vector_size, pColCount));
      
      *mOutput << mName << ": resized to " << mMeanVector.row_count() << "x" << mMeanVector.col_count() << " (removed margin).\n";
      mMeanVector.clear();
      
      mMeanValue.set_meta(H("sample_offset"), mRowMargin); // shift display window right / left
      mMeanValue.set_meta(H("sample_count"),  pRowCount);  // total number of samples per window when computing width_ratio
    }
    
    return true;
  }
  
  class_recorder_states_t mState;
  
  std::string mFolder;        /**< Folder containing the class data.   */
  std::string mClassFile;     /**< Current class data file.            */
  FILE * mTrainFile;

  bool mUseSnap;              /**< True if the recording should snap to the best match. Usually better without. */
  
  size_t mSnapOffset;         /**< Best match with this offset in mBuffer.                                      */
  size_t mRowOffset;          /**< Offset for recorded class inside full data.                                  */
  
  bool      mHasLiveData;     /**< Make sure we record new data. */
  const Matrix * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls.      */
  CutMatrix mLiveView;        /**< Live stream view. Points inside mLiveBuffer.                                 */
  Value    mLiveValue;      /**< Used to send mLiveView                                                       */
  
  Matrix mMeanVector;         /**< Store the mean value for all vectors from this class.                        */
  Value mMeanValue;         /**< Used to send mean value.                                                     */
  
  Matrix mBuffer;             /**< Store a single vector +  margin.                                             */
  CutMatrix mView;            /**< Resulting view of the data used to record to file (points inside mBuffer).   */
  Value mBufferValue;       /**< Used to send view matrix.                                                    */
  real_t mMargin;             /**< Size (in %) of the margin.                                                   */
  size_t mRowMargin;          /**< Number of rows on each side of the vector (mBuffer.row_count() * margin/2).  */
  size_t mVectorCount;        /**< Number of vectors used to build the current mean value.                      */
  int mTempo;                 /**< Tempo for countdown and recording.                                           */

  int mClassLabel;            /**< Current label. Used during recording and recognition.                        */
  int mSampleRate;            /**< Number of samples per second (used to compute recording time).               */
  int mChannel;               /**< Midi channel used to send notes. */
};


extern "C" void init()
{
  CLASS (ClassRecorder)
  OUTLET(ClassRecorder,current)
  OUTLET(ClassRecorder,mean)
  OUTLET(ClassRecorder,countdown)
}