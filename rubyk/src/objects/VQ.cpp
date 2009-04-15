#include "rubyk.h"
#include <float.h>  // DBL_MAX
#define INITIAL_TRAINING_DATA_SIZE 512

/** Svm states. */
enum vq_states_t {
  Waiting,     /**< Waiting for command ('r'=Record, 'l'=Learn, '\n'=Label) */
  Recording,   /**< Writes each input vector in a file. */
  Learning,    /**< Computing codevectors in separate thread. */
  Label,       /**< Sends a label for each input vector. */
};

#include "vq/elbg.h"
/** Vector Quantization (reduce large vectors to an integer representing the index in the codebook). 
  * Uses the Enhanced LBG Algorithm implemented by the guys of FFmpeg project. */
class VQ : public Node
{
public:
  ~VQ()
  {
    mState = Waiting;
    if (mThread) pthread_join( mThread, NULL); // wait for child to finish
    
    if (mTrainFile) {
      fclose(mTrainFile);
      mTrainFile = NULL;
    }
  }
  
  bool init ()
  {
    mTrainingData.set_sizes(0,8);
    mCodebook.set_sizes(8,8);
    mFolder = "data";
    
    mThread    = NULL;
    mTrainFile = NULL;
    
    return true;
  }
  
  bool set (const Value &p)
  {
    size_t col_count = p.val("vector", mCodebook.col_count());
    size_t row_count = p.val("resolution", mCodebook.row_count());
    
    if (col_count != mCodebook.col_count() || row_count != mCodebook.row_count()) {
      size_t i = 1;
      while(i < row_count) i *= 2;
      if (i != row_count) {
        *output_ << name_ << ": incorrect resolution size " << row_count << " should be a power of 2 (" << i << ").\n";
        return false;
      }
      if(!mCodebook.set_sizes(row_count,col_count)) {
        *output_ << name_ << ": codebook (" << mCodebook.error_msg() << ").\n";
        return false;
      }
    }

    mScale        = p.val("scale", (real_t)mCodebook.col_count());   /**< Value to multiply reals before scaling to integers. */
    mFolder       = p.val("data", mFolder);
    
    mState = Waiting;
    enter(Label);
    
    return true;
  }
  
  // inlet 1
  void bang(const Value &val)
  { 
    int cmd;
    if (!is_ok_) return; // only recover after 'set'
    
    switch(mState) {
    case Waiting:
      if (val.get(&cmd)) {
        do_command(cmd);
      }  
      break;
    case Recording:
      if (val.get(&cmd)) {
        do_command(cmd);
      } else if (val.type == MatrixValue) {
        if (val.matrix.value->col_count() == mCodebook.col_count()) {
          if (!mTrainFile) return;
          
          val.matrix.value->to_file(mTrainFile);
          mTrainingData.cast_append(val.matrix.value->data, val.matrix.value->size(), mScale);
          if (mDebug) *output_ << name_ << ": recorded vector '" << mTrainingData.row_count() << "'.\n";
        } else {
          *output_ << name_ << ": wrong signal size '" << val.matrix.value->col_count() << "'. Should be '" << mCodebook.col_count() << "'.\n";
        }
      }
      break;
    case Learning:
      if (val.get(&cmd)) {
        if (cmd == 'a') {
          *output_ << name_ << ": aborting learn.\n";
          mState = Waiting;
          // wait for thread to finish
          if (mThread) pthread_join( mThread, NULL); // wait for child to finish
          mThread = NULL;
        } else {
          *output_ << name_ << ": type 'a' to abort learning phase.\n";
        }
      } else {
        // ignore other inputs.
      }
      break;
    case Label:
      if (val.type == MatrixValue) {
        if (val.matrix.value->col_count() == mCodebook.col_count()) {
          int label = label_for(*val.matrix.value);
          send(label / (real_t)mCodebook.row_count());
        } else {
          *output_ << name_ << ": wrong signal size '" << val.matrix.value->col_count() << "'. Should be '" << mCodebook.col_count() << "'.\n";
        }
      } else if (val.get(&cmd)) {
        do_command(cmd);
      }
    }
  }
  
  virtual const Value inspect(const Value &val) 
  {  
    bprint(mSpy, mSpySize,"%ix%i", mCodebook.row_count(), mCodebook.col_count());    
  }
private:
  
  virtual std::string model_file_path()
  {
    std::string path(mFolder);
    path.append("/").append(name_).append(".model");
    return path;
  }
  
  int label_for(const Matrix& pVector)
  {
    // find best matching vector from codebook
    // FIXME: there are much better algorithms then full search !
    // Use QccPack ?
      
    // 2. match codebook
    size_t match_index = 0;
    Real match_distance = DBL_MAX;
    Real distance;
    Real * codeword;
    Real * vector = pVector.data;
    for(size_t i=0; i < mCodebook.row_count(); i++) {
      codeword = mCodebook[i];
      distance = 0;
      for(size_t j=0; j <  mCodebook.col_count(); j++) {
        distance += (vector[j] - codeword[j]) * (vector[j] - codeword[j]);
        if (distance > match_distance) break;
      }
    
      if (distance < match_distance) {
        match_index = i;
        match_distance = distance;
      }
    }
    
    return match_index;
  }
  
  std::string train_file_path()
  {
    std::string path(mFolder);
    path.append("/").append(name_).append(".train");
    return path;
  }
  
  /** Load codebook from file. */
  bool load_model()
  {
    FILE * file;
    file = fopen(model_file_path().c_str(), "rb");
      if (!file) {
        *output_ << name_ << ": could not open codebook data '" << model_file_path() << "'\n.";
        return false;
      }
      
      if(!mCodebook.from_file(file)) {
        *output_ << name_ << ": could not load codebook from '" << model_file_path() << "' (" << mCodebook.error_msg() << ")\n";
        fclose(file);
        return false;
      }
    fclose(file);
    return true;
  }
  
  void enter(vq_states_t state)
  {
    // cleanup
    if (mTrainFile) fclose(mTrainFile);
    if (mThread) {
      int old_sate = mState;
      mState = Waiting;
      pthread_join( mThread, NULL); // wait for child to finish
      mState = old_sate;
    }
    
    switch(state) {
    case Label:
      if (load_model()) {
        mState = Label;
      } else {
        *output_ << name_ << ": could not start to label (could not load codebook).\n";
      }
      break;
    case Recording:
      // 1. open training file
      if (!mTrainFile) {
        mTrainFile = fopen(train_file_path().c_str(), "wb"); // each time we record, we start reset the database
        if (!mTrainFile) {
          *output_ << name_ << ": could not open '" << train_file_path() << "' to store training data.\n";
          return;
        }
      }  
      if(!mTrainingData.set_sizes(0,mCodebook.col_count())) {
        *output_ << name_ << ": training matrix (" << mTrainingData.error_msg() << ").\n";
        return;
      }
      *output_ << name_ << ": recording started.\n";
      mState = Recording;
      break;
    case Learning:
      mState = Learning;
      *output_ << name_ << ": training started.\n";
      pthread_create( &mThread, NULL, &VQ::call_train, (void*)this);
      break;
    case Waiting:
      mState = Waiting;
    }
  }

  /** (John) */
  static void* call_train(void * node)
  {
    // runs in new thread
    ((VQ*)node)->train();
    return NULL;
  }
  
  /** Compute codebook from the training database. 
    * FIXME: find a way to read mState in order to abort if mState is no longer Learning. */
  void train()
  {
    Matrix vector;
    IntMatrix int_codebook;
    int * nearest_cb;
    
    if (!mTrainingData.row_count()) {
      // 1. open file containing data
      if (mTrainFile) fclose(mTrainFile);
      mTrainFile = fopen(train_file_path().c_str(), "rb");
        if (!mTrainFile) {
          *output_ << name_ << ": could not open '" << train_file_path() << "' to read training data.\n";
          return;
        }
        
        if(!mTrainingData.set_sizes(0,mCodebook.col_count())) {
          *output_ << name_ << ": training matrix (" << mTrainingData.error_msg() << ").\n";
          return;
        }
        // 2. load all training vectors in memory
        while(vector.from_file(mTrainFile)) {
          mTrainingData.cast_append(vector.data, vector.size(), mScale);
          *output_ << name_ << ": loaded training vector %i\n", mTrainingData.row_count();
        }
      fclose(mTrainFile);
      
      mTrainFile = NULL;
    }
    
    if(mTrainingData.row_count() < mCodebook.row_count()) {
      *output_ << name_ << ": number of training vectors (" << mTrainingData.row_count() << ") too small compared to codebook size (" << mCodebook.row_count() << ").\n";
      return;
    }
    
    *output_ << name_ << ": building codebook from " << mTrainingData.row_count() << " training vectors.\n";
    
    // 3. allocate nearest_cb used by ff_do_elbg. Sets the nearest codebook entry for each training vector.
    if(!alloc_ints(&nearest_cb, mTrainingData.row_count(), "nearest codebook")) return;
    
    if(!int_codebook.set_sizes(mCodebook.row_count(), mCodebook.col_count())) {
      *output_ << name_ << ": integer codebook (" << mCodebook.error_msg() << ")\n";
      return;
    }
    
    memset(nearest_cb, 0, mTrainingData.row_count());
    
    // 3. build codebook
    
    // initialization
    AVRandomState rand_state;
    av_init_random(1234, &rand_state); // seed doesn't matter since we just need uniform distribution, not crypto
    ff_init_elbg(mTrainingData.data, mTrainingData.col_count(), mTrainingData.row_count(), int_codebook.data, int_codebook.row_count(), 2, nearest_cb, &rand_state);
    
    // generate codebook
    ff_do_elbg  (mTrainingData.data, mTrainingData.col_count(), mTrainingData.row_count(), int_codebook.data, int_codebook.row_count(), 2, nearest_cb, &rand_state);
    
    // 4. cleanup
    free(nearest_cb);
    
    // 5. copy integer codebook to Real codebook
    if (!mCodebook.set_sizes(0, int_codebook.col_count())) {
      *output_ << name_ << ": mCodebook (" << mCodebook.error_msg() << ")\n";
      return;
    }
    
    if(!mCodebook.cast_append(int_codebook.data, int_codebook.size(), 1.0 / mScale)) {
      *output_ << name_ << ": could not copy integer codebook to reals codebook (" << mCodebook.error_msg() << ")\n";
      return;
    }
    
    // 5. save to file
    FILE * file = fopen(model_file_path().c_str(), "wb");
      if (!file) {
        *output_ << name_ << ": could not write codebook to '" << model_file_path() << "'\n.";
        return;
      }
      *output_ << name_ << ": writing " << mCodebook.row_count() << " x " << mCodebook.col_count() << " to file.\n";
      if(!mCodebook.to_file(file)) {
        *output_ << name_ << ": could not write mCodebook to file (" << mCodebook.error_msg() << ")\n";
        return;
      };
    fclose(file);
    
    if (mState == Learning)
      *output_ << name_ << ": training complete.\n";
    mState = Label; // training done
  }
  
  void do_command(int cmd)
  {
    if (cmd == 'r') {
      enter(Recording);
    } else if (cmd == '\n') {
      enter(Label);
    } else if (cmd == 'l') {
      enter(Learning);
    } else {
      *output_ << name_ << ": invalid command '" << (char)cmd << "'. Use 'l' to start learning, '\\n' to start label and 'r' to record.\n";
    }
  }
 
  int       mState;        /**< Current state of the vector quantizer (Recording, Learning, Label) */
  pthread_t mThread;       /**< Used for learning. */
  std::string mFolder;     /**< Where to store training data, codebook vectors. */
  
  Real    mScale;        /**< Value to multiply reals before conversion to integer. */
  Matrix    mCodebook;     /**< Matrix of mCodebookSize x mVectorSize. */
  IntMatrix mTrainingData; /**< Matrix of mTrainingSize x mVectorSize. Used during training. */
  FILE *    mTrainFile;    /**< Where the training vectors are stored. */
};

extern "C" void init()
{
  CLASS(VQ)
  OUTLET(VQ, label)
  OUTLET(VQ, probabilities)
}