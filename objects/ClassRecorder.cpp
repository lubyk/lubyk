
#include "class.h"
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
  
  bool init(const Params& p)
  {
    // defaults
    mTempo       = 120;
    mSampleRate  = 256;
    mMargin      = 2.0;
    mFolder      = "data";
    mUseSnap     = true;
    mClassLabel  = 0;
    mVectorCount = 0;
    mTrainFile   = NULL;
    mMeanSignal.set(mMeanVector);
    mLiveSignal.set(mLiveView);
    mS.set(mView);

    if (!resize(4,8)) return false;
    
    enter(ReadyToRecord);
    return true;
  }

  bool set(const Params& p)
  {
    // vector sizes is set from incomming stream
    p.get(&mTempo     ,"tempo");
    p.get(&mSampleRate,"rate");
    p.get(&mMargin    ,"margin");
    p.get(&mFolder    ,"data");
    p.get(&mUseSnap   ,"snap");
    
    return true;
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    int cmd;
    
    if (!mIsOK) return; // no recovery
    
    if (sig.type == MatrixSignal) {
      if (!resize(sig.matrix.value->row_count(), sig.matrix.value->col_count())) return;
      sig.get(&mLiveBuffer);
    } else {
      time_t record_time = (time_t)(ONE_SECOND * mBuffer.row_count())/(mBuffer.col_count() * mSampleRate);
      time_t record_with_margin = record_time * (1 + mMargin); //  * (1 + mMargin/2.0) ???
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
          if (mView.data != mBuffer.data) {
            if (!mView.set_view(mBuffer, mRowMargin, mRowMargin + mMeanVector.row_count() - 1)) {
              *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
              return;
            }
            *mOutput << mName << ": no-snap\n~> ";
          } else {
            if (!mView.set_view(mBuffer, mVectorOffset, mVectorOffset + mMeanVector.row_count() - 1)) {
              *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
              return;
            }
            *mOutput << mName << ": snap\n~> ";
          }
          break;
        } else if (cmd == RK_RIGHT_ARROW) { // -> right arrow 301
          mVectorOffset += mBuffer.col_count();
          if (mVectorOffset > mBuffer.row_count() - mMeanVector.row_count()) mVectorOffset = mBuffer.row_count() - mMeanVector.row_count();
          
          if (!mView.set_view(mBuffer, mVectorOffset, mVectorOffset + mMeanVector.row_count() - 1)) {
            *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
            return;
          }
          break;
        } else if (cmd == RK_LEFT_ARROW) { // <- left arrow  302
          mVectorOffset -= mBuffer.col_count();
          if (mVectorOffset < 0) mVectorOffset = 0;
          
          if (!mView.set_view(mBuffer, mVectorOffset, mVectorOffset + mMeanVector.row_count() - 1)) {
            *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
            return;
          }
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
    send(mMeanSignal, 2);
    
    if (mState == Validation)
      send(mS);        // recorded signal
    else
      send(mLiveSignal); // live signal
  }
  
private:
  
  void receive_data()
  {
    if (!mHasLiveData) {
      *mOutput << mName << ": no data to record (no stream coming from inlet 1).\n";
      return;
    }
    
    // copy data into our local buffer
    mBuffer.copy(*mLiveBuffer);
    
    if (mRowMargin) {
      // try to find the best bet by calculating minimal distance
      double distance, min_distance = -1.0;
      double d;
      double * vector;
      double * mean = mMeanVector.data;
      int   delta_used = mBuffer.row_count() - mMeanVector.row_count();
      for(size_t j = mBuffer.row_count() - mMeanVector.row_count(); j >= 0; j--) {
        distance = 0.0;
        vector = mBuffer[j];
        for(size_t i=0; i < mBuffer.col_count(); i++) {
          d = (mean[i] - vector[i]) ;
          if (d > 0)
            distance += d;
          else
            distance -= d;
        }
        distance = distance / mBuffer.col_count();
        if (min_distance < 0 || distance < min_distance) {
          delta_used = j;
          min_distance = distance;
        }
      }
      mVectorOffset = delta_used;
      *mOutput << mName << ": distance to mean vector " << min_distance << " (delta " << delta_used << "/" << mBuffer.row_count() - mMeanVector.row_count() << ")\nKeep ? ~> ";
      fflush(stdout); // FIXME: should be related to *mOutput
    } else {
      mVectorOffset = mBuffer.row_count() - mMeanVector.row_count();
      *mOutput << mName << ":~> Keep ? ";
      fflush(stdout); // FIXME: should be related to *mOutput
    }
    
    if (mUseSnap) {
      if (!mView.set_view(mBuffer, mVectorOffset, mVectorOffset + mMeanVector.row_count() - 1)) {
        *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
        return;
      }
    } else {
      
      if (!mView.set_view(mBuffer, mRowMargin, mRowMargin + mMeanVector.row_count() - 1)) {
        *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
        return;
      }
    }
  }
  
  void store_data()
  {
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
    *mOutput << mName << ": recording vector " << mVectorCount + 1 << " for " << cmd << "\n~> ";
  }
  
  void enter(class_recorder_states_t pState)
  {
    switch(pState) {
    case ReadyToRecord:
      mState = pState;
      *mOutput << mName << ": Ready to record\n~> ";
      break;
    case Recording:
      mHasLiveData = false;
      break;
    default:
      mState = pState;
    }
  }
  
  /** Update the mean value with the current vector. */
  void update_mean_value(const Matrix& pVector)
  {
    mVectorCount++;
    double map = (double)(mVectorCount - 1) / (double)(mVectorCount);  // avg = (avg * (n-1)/n) + value/n
    mMeanVector *= map;
    mMeanVector.add(pVector, 0, -1, 1.0 / mVectorCount);
  }
  
  /** Execute the function for each vector contained in the class. */
  void load_class(int cmd, void (ClassRecorder::*function)(const Matrix& pVector))
  {
    Matrix vector;
    vector.set_sizes(1, mMeanVector.col_count());
    
    mClassLabel = cmd;
    /** reset mean value. */
    mVectorCount = 0;
    mMeanVector.clear();
    
    // 1. find file
    mClassFile = mFolder;
    std::string str;
    bprint(str,"/class_%i.txt", cmd);
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
  }
  
  // resize internal buffers from incomming stream
  bool resize(size_t pRowCount, size_t pColCount)
  {
    if (mBuffer.row_count() != pRowCount || mBuffer.col_count() != pColCount) {
      size_t vector_size = pRowCount / (1.0 + mMargin);
      mRowMargin = (pRowCount - vector_size) / 2;
      
      if(!mBuffer.set_sizes(pRowCount, pColCount)) {
        *mOutput << mName << ": mBuffer (" << mBuffer.error_msg() << ").\n";
        return false;
      }
      if (!mMeanVector.set_sizes(vector_size, pColCount)) {
        *mOutput << mName << ": mMeanVector (" << mMeanVector.error_msg() << ").\n";
        return false;
      }
      if (!mView.set_view(mBuffer, mRowMargin, mRowMargin + mMeanVector.row_count() - 1)) {
        *mOutput << mName << ": mView (" << mView.error_msg() << ").\n";
        return false;
      }
      if (!mLiveView.set_view(*mLiveBuffer, -mMeanVector.row_count(), -1)) {
        *mOutput << mName << ": mLiveView (" << mLiveView.error_msg() << ").\n";
        return false;
      }
      *mOutput << mName << ": resized to " << mMeanVector.row_count() << "x" << mMeanVector.col_count() << " (removed margin).\n";
      mMeanVector.clear();
    }
    return true;
  }
  
  class_recorder_states_t mState;
  
  std::string mFolder;        /**< Folder containing the class data.   */
  std::string mClassFile;     /**< Current class data file.            */
  FILE * mTrainFile;

  bool mUseSnap;              /**< True if the recording should snap to the best match. Usually better without. */
  
  size_t mVectorOffset;       /**< Best match with this offset in mBuffer.                                      */
  
  bool      mHasLiveData;     /**< Make sure we record new data. */
  const Matrix * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls.      */
  CutMatrix mLiveView;        /**< Live stream view. Points inside mLiveBuffer.                                 */
  Signal    mLiveSignal;      /**< Used to send mLiveView                                                       */
  
  Signal mMeanSignal;         /**< Used to send mean value.                                                     */
  Matrix mMeanVector;         /**< Store the mean value for all vectors from this class.                        */
  
  Matrix mBuffer;             /**< Store a single vector +  margin.                                             */
  CutMatrix mView;            /**< Resulting view of the data (points inside mBuffer).                          */
  double mMargin;             /**< Size (in %) of the margin.                                                   */
  size_t mRowMargin;          /**< Number of rows on each side of the vector (mBuffer.row_count() * margin/2).  */
  size_t mVectorCount;        /**< Number of vectors used to build the current mean value.                      */
  int mTempo;                 /**< Tempo for countdown and recording.                                           */

  int mClassLabel;            /**< Current label. Used during recording and recognition.                        */
  int mSampleRate;            /**< Number of samples per second (used to compute recording time).               */
};


extern "C" void init()
{
  CLASS (ClassRecorder)
  OUTLET(ClassRecorder,current)
  OUTLET(ClassRecorder,mean)
  OUTLET(ClassRecorder,countdown)
}