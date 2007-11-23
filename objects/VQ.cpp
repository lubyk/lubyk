#include "class.h"
#include <limits.h>  // INT_MAX
#define INITIAL_TRAINING_DATA_SIZE 512

/** Svm states. */
enum quantize_states_t {
  Waiting,     /**< Waiting for command ('r'=Record, 'l'=Learn, '\n'=Label) */
  Recording,   /**< Writes each input vector in a file. */
  Learning,    /**< Computing codevectors in separate thread. */
  Label,       /**< Sends a label for each input vector. */
};

#include "VQ/elbg.h"
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
    if (mTrainingData) free(mTrainingData);
    if (mCodebook)     free(mCodebook);
    if (mVector)       free(mVector);
  }
  
  bool init (const Params& p)
  {
    mVectorSize   = p.val("size", 8);        /**< Dimension of each vector. */
    mCodebookSize = p.val("resolution", 16); /**< Codebook size. The result will be an integer between
                                               *  [1..resolution]. Must be a power of 2. */
    mScale        = p.val("scale", 256.0);   /**< Value to multiply doubles before scaling to integers. */
    mFolder       = p.val("store", std::string("quantize_data"));
    
    mThread    = NULL;
    mTrainFile = NULL;
    
    /** Allocate codebook. */
    mCodebook = (int*)malloc(mCodebookSize * mVectorSize * sizeof(int));
    if (!mCodebook) {
      *mOutput << mName << ": could not allocate " << mCodebookSize * mVectorSize << " integers for codebook.\n";
      return false;
    }
    
    /** Allocate mVector. */
    mVector = (int*)malloc(mVectorSize * sizeof(int));
    if (!mCodebook) {
      *mOutput << mName << ": could not allocate " << mVectorSize << " integers for encoding vector.\n";
      return false;
    }
    
    mState = Waiting;
    enter(Label);
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    int cmd;
    if (!mIsOK) return; // no recovery
    
    switch(mState) {
    case Waiting:
      if (sig.get(&cmd)) {
        do_command(cmd);
      }  
      break;
    case Recording:
      if (sig.get(&cmd)) {
        do_command(cmd);
      } else if (sig.type == ArraySignal) {
        if (sig.array.size == mVectorSize) {
          write_vector(sig.array.value);
          if (mDebug) *mOutput << mName << ": recorded vector '" << mTrainingSize << "'.\n";
        } else {
          *mOutput << mName << ": wrong signal size '" << sig.array.size << "'. Should be '" << mVectorSize << "'.\n";
        }
      }
      break;
    case Learning:
      if (sig.get(&cmd)) {
        if (cmd == 'a') {
          *mOutput << mName << ": aborting learn.\n";
          mState = Waiting;
          // wait for thread to finish
          if (mThread) pthread_join( mThread, NULL); // wait for child to finish
          mThread = NULL;
        } else {
          *mOutput << mName << ": type 'a' to abort learning phase.\n";
        }
      } else {
        // ignore other inputs.
      }
      break;
    case Label:
      if (sig.type == ArraySignal) {
        if (sig.array.size == mVectorSize) {
          int label = label_for(sig.array.value);
          if (mDebug) *mOutput << mName << ": " << label << "\n";
          send(label / (double)mCodebookSize);
        } else {
          *mOutput << mName << ": wrong signal size '" << sig.array.size << "'. Should be '" << mVectorSize << "'.\n";
        }
      } else if (sig.get(&cmd)) {
        do_command(cmd);
      }
    }
  }
  
private:
  
  int label_for(double * pVector)
  {
    if (!mVector) return 0;
    
    // find best matching vector from codebook
    // FIXME: there are much better algorithms then full search !
    // Use QccPack ?
    
    // 1. scale to integers (Could be avoided if we store the codebook in integer)
    for (int i=0; i < mVectorSize; i++)
      mVector[i] = (int)(pVector[i] * mScale);
      
    // 2. match codebook
    int match_index = 0;
    long match_distance = INT_MAX;
    long distance;
    int * codeword;
    for(int i=0; i < mCodebookSize; i++) {
      codeword = mCodebook + mVectorSize * i;
      distance = 0;
      for(int j=0; j < mVectorSize; j++) {
        distance += (mVector[j] - codeword[j]) * (mVector[j] - codeword[j]);
        if (distance > match_distance) break;
      }
      if (distance < match_distance) {
        match_index = i;
        match_distance = distance;
      }
      // FIXME: add another clause to drop search if distance is small enough
    }
    
    return match_index;
  }
  
  std::string codebook_path()
  {
    std::string path(mFolder);
    path.append("/quantize.book");
    return path;
  }
  
  std::string train_file_path()
  {
    std::string path(mFolder);
    path.append("/quantize.train");
    return path;
  }
  
  /** Load codebook from file. */
  bool load_codebook()
  {
    FILE * file;
    file = fopen(codebook_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": could not open codebook data '" << codebook_path() << "'\n.";
        return false;
      }
      
      int vector_count = 0;
      float val;
      while(vector_count < mCodebookSize) {
        for(int i=0; i < mVectorSize; i++) {
          // read a float
          if(fscanf(file, " %f", &val) == EOF) {
            *mOutput << mName << ": file format reading codebook vector " << vector_count+1 << " error.\n";
            return false;
          }
          fscanf(file, "\n"); // ignore newline
          mCodebook[vector_count * mVectorSize + i] = (int)(val * mScale);
        }
        printf("Loaded %i\n", vector_count+1);
        vector_count++;
      }
    fclose(file);
    return true;
  }
  
  /** Save a vector to the training database (on file and in memory). */
  void write_vector(double * vector)
  {
    if (!mTrainFile) return;
    for(int i=0; i < mVectorSize; i++)
      fprintf(mTrainFile, " % .5f", (float)vector[i]);
      
    fprintf(mTrainFile, "\n");
    
    add_to_database(vector);
  }
  
  void enter(quantize_states_t state)
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
      if (load_codebook()) {
        mState = Label;
      } else {
        *mOutput << mName << ": could not start to label (could not load codebook).\n";
      }
      break;
    case Recording:
      // 1. open training file
      if (!mTrainFile) {
        mTrainFile = fopen(train_file_path().c_str(), "wb"); // each time we record, we start reset the database
        if (!mTrainFile) {
          *mOutput << mName << ": could not open '" << train_file_path() << "' to store training data.\n";
          return;
        }
      }
      mTrainingSize = 0;
      *mOutput << mName << ": recording started.\n";
      mState = Recording;
      break;
    case Learning:
      mState = Learning;
      *mOutput << mName << ": training started.\n";
      pthread_create( &mThread, NULL, &VQ::call_train, (void*)this);
      break;
    }
  }

  /** (John) */
  static void* call_train(void * node)
  {
    // runs in new thread
    ((VQ*)node)->train();
  }
  
  /** Compute codebook from the training database. 
    * FIXME: find a way to read mState in order to abort if mState is no longer Learning. */
  void train()
  {
    if (!mTrainingSize) {
      // 1. open file containing data
      if (mTrainFile) fclose(mTrainFile);
      mTrainFile = fopen(train_file_path().c_str(), "rb");
        if (!mTrainFile) {
          *mOutput << mName << ": could not open '" << train_file_path() << "' to read training data.\n";
          return;
        }
    
        // 2. load all training vectors in memory
        mTrainingSize = 0;
        while(load_training_vector());
      fclose(mTrainFile);
      
      mTrainFile = NULL;
    
    }
    
    if(mTrainingSize < mCodebookSize) {
      *mOutput << mName << ": number of training vectors (" << mTrainingSize << ") too small compared to codebook size.\n";
      return;
    }
    
    *mOutput << mName << ": building codebook from " << mTrainingSize << " training vectors.\n";
    
    // 3. allocate nearest_cb used by ff_do_elbg. Sets the nearest codebook entry for each training vector.
    int * nearest_cb = (int*)malloc(mVectorSize * mTrainingSize * sizeof(int));
    if (!nearest_cb) {
      *mOutput << mName << ": could not allocate " << mVectorSize * mTrainingSize << " integers for nearest_cb.\n";
      return;
    }
    
    memset(nearest_cb, 0, mTrainingSize);
    // 3. build codebook
    
    // initialization
    AVRandomState rand_state;
    av_init_random(1234, &rand_state); // seed doesn't matter since we just need uniform distribution, not crypto
    ff_init_elbg(mTrainingData, mVectorSize, mTrainingSize, mCodebook, mCodebookSize, 2, nearest_cb, &rand_state);
    
    // generate codebook
    ff_do_elbg  (mTrainingData, mVectorSize, mTrainingSize, mCodebook, mCodebookSize, 2, nearest_cb, &rand_state);
    
    // 4. cleanup
    free(nearest_cb);
    
    // 5. save to file
    FILE * file = fopen(codebook_path().c_str(), "wb");
      if (!file) {
        *mOutput << mName << ": could not write codebook to '" << codebook_path() << "'\n.";
        return;
      }
      printf("Writing %i x %i to file.\n", mCodebookSize, mVectorSize);
      for(int i=0; i < mCodebookSize; i++) {
        for(int j=0; j < mVectorSize; j++) {
          printf(" %i", mCodebook[i * mVectorSize + j]);
          fprintf(file, " %.5f", ((float)mCodebook[i * mVectorSize + j])/mScale);
        }
        printf("\n");
        fprintf(file, "\n");
      }
    fclose(file);
    
    if (mState == Learning)
      *mOutput << mName << ": training complete.\n";
    mState = Label; // training done
  }
  
  bool load_training_vector()
  {
    int * vector = alloc_vector_pointer(); // write directly in training buffer
    if (!vector) return false;
    
    float val;
    for(int i=0; i < mVectorSize; i++) {
      // read a float
      if(fscanf(mTrainFile, " %f", &val) == EOF) {
        if (i != 0) // premature end of data
          *mOutput << mName << ": file format reading training vector " << mTrainingSize+1 << " error.\n";
        return false;
      }
      fscanf(mTrainFile, "\n"); // ignore newline
      vector[i] = (int)(mScale * val);
    }
    
    printf("Loaded train %i\n", mTrainingSize+1);
    mTrainingSize++;
    return true;
  }
  
  void add_to_database(double * pVector)
  {
    int * vector = alloc_vector_pointer();
    if (!vector) return;
    
    for(int i=0; i < mVectorSize; i++) vector[i] = (int)(mScale * pVector[i]);
    
    mTrainingSize++;
  }
  
  int * alloc_vector_pointer()
  {
    if(!mTrainingData) {
      // allocate a first buffer
      mTrainingDataSize = INITIAL_TRAINING_DATA_SIZE;
      mTrainingData = (int*)malloc(mTrainingDataSize * mVectorSize * sizeof(int));
      if (!mTrainingData) {
        *mOutput << mName << ": could not allocate " << mTrainingDataSize * mVectorSize << " integers for training data.\n";
        return NULL;
      }
    } else if (mTrainingSize >= mTrainingDataSize) {
      // training data buffer too small, reallocate
      int * new_buf;
      mTrainingDataSize *= 2;
      new_buf = (int*)realloc(mTrainingData, mTrainingDataSize * mVectorSize * sizeof(int));
      if (!new_buf) {
        *mOutput << mName << ": could not reallocate " << mTrainingDataSize * mVectorSize << " integers for training data.\n";
        return NULL;
      } else mTrainingData = new_buf;
    }
    return mTrainingData + (mTrainingSize * mVectorSize);
  }
  
  void free_buffer(int ** pBuffer, int pMajorSize)
  {
    for(int i=0; i < pMajorSize; i++) {
      if (!pBuffer[i]) continue;
      free(pBuffer[i]);
    }
    free(pBuffer);
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
      *mOutput << mName << ": invalid command '" << (char)cmd << "'. Use 'l' to start learning, '\\n' to start label and 'r' to record.\n";
    }
  }
 
  int       mState;        /**< Current state of the vector quantizer (Recording, Learning, Label) */
  pthread_t mThread;       /**< Used for learning. */
  std::string mFolder;     /**< Where to store training data, codebook vectors. */
  
  int       mVectorSize;   /**< Dimension of a vector. */
  int  *    mVector;       /**< Integer version of the live vector used during encoding. */
  double    mScale;        /**< Value to multiply doubles before conversion to integer. */
  int  *    mCodebook;     /**< Array of mCodebookSize pointers to vectors of size mVectorSize as integers. */
  int  *    mTrainingData; /**< Array of mTrainingSize pointers to vectors of size mVectorSize as integers. */
  FILE *    mTrainFile;    /**< Where the training vectors are stored. */
  int       mTrainingDataSize;  /**< Size of mTrainingData buffer. */
  int       mTrainingSize;  /**< Number of training vectors. */
  int       mCodebookSize; /**< Number of prototypes. */
};

extern "C" void init()
{
  CLASS(VQ)
  OUTLET(VQ, label)
}