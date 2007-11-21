#include "class.h"

#define INITIAL_TRAINING_DATA_SIZE 1024

/** Svm states. */
enum quantize_states_t {
  Waiting,     /**< Waiting for command ('r'=Record, 'l'=Learn, '\n'=Label) */
  Recording,   /**< Writes each input vector in a file. */
  Learning,    /**< Computing codevectors in separate thread. */
  Label,       /**< Sends a label for each input vector. */
};


/** Vector Quantization (reduce large vectors to an integer representing the index in the codebook). */
class Quantize : public Node
{
public:
  
  ~Quantize()
  {
    mState = Waiting;
    if (mThread) pthread_join( mThread, NULL); // wait for child to finish
    
    if (mTrainFile) {
      fclose(mTrainFile);
      mTrainFile = NULL;
    }
    if (mTrainingData) free_buffer(mTrainingData, mTrainingDataSize);
    if (mCodebook)     free_buffer(mTrainingData, mCodebookSize);
  }
  
  bool init (const Params& p)
  {
    mVectorSize   = p.val("size", 8);        /**< dimension of vector (number of doubles per vector). */
    mCodebookSize = p.val("resolution", 16); /**< codebook size. The result will be an integer between [1..resolution]. */
    mFolder       = p.val("store", std::string("quantize_data"));
    
    mThread    = NULL;
    mTrainFile = NULL;
    /** Allocate codebook array. */
    mCodebook = (double**)malloc(mCodebookSize * sizeof(double*));
    if (!mCodebook) {
      *mOutput << mName << ": could not allocate " << mCodebookSize << " doubles pointers for codebook.\n";
      return false;
    }
    
    /** Allocate memory for each vector in the codebook. */
    for(int i=0; i < mCodebookSize; i++) {
      mCodebook[i] = (double*)malloc(mVectorSize * sizeof(double));
      if (!mCodebook[i]) {
        *mOutput << mName << ": could not allocate " << mVectorSize << " doubles for codebook vector " << i << " .\n";
        return false;
      }
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
        if (cmd == '\e') {
          *mOutput << mName << ": aborting learn.\n";
          mState = Waiting;
          // wait for thread to finish
          if (mThread) pthread_join( mThread, NULL); // wait for child to finish
          mThread = NULL;
        } else {
          *mOutput << mName << ": type 'ESC' to abort learning phase.\n";
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
          send(label);
        } else {
          *mOutput << mName << ": wrong signal size '" << sig.array.size << "'. Should be '" << mVectorSize << "'.\n";
        }
      }
    }
  }
  
private:
  
  int label_for(double * vector)
  {
    // find best matching vector from codebook
    // TODO
    return 0;
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
    mCodebookSize = 0;
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
          if(fscanf(file, " %f", &val) <= 0) {
            *mOutput << mName << ": file format reading codebook vector " << vector_count+1 << " error.\n";
            return false;
          }
          fscanf(file, "\n"); // ignore newline
          mCodebook[vector_count][i] = (double)val;
          vector_count++;
        }
      }
    fclose(file);
    return true;
  }
  
  /** Save a vector to the training database. */
  void write_vector(double * vector)
  {
    if (!mTrainFile) return;
    for(int i=0; i < mVectorSize; i++)
      fprintf(mTrainFile, " %.5f", (float)vector[i]);
      
    fprintf(mTrainFile, "\n");
    mTrainingSize++;    
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
      mState = Recording;
      break;
    case Learning:
      mState = Learning;
      pthread_create( &mThread, NULL, &Quantize::call_train, (void*)this);
      break;
    }
  }

  /** (John) */
  static void* call_train(void * node)
  {
    // runs in new thread
    ((Quantize*)node)->train();
  }
  
  /** Compute codebook from the training database. */
  void train()
  {
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
    
    // 3. train
    
    // 4. cleanup ?
  }
  
  bool load_training_vector()
  {
    // 0. allocate memory
    if(!mTrainingData) {
      // allocate a first vector
      mTrainingDataSize = INITIAL_TRAINING_DATA_SIZE;
      mTrainingData = (double**)malloc(mTrainingDataSize * sizeof(double*));
      if (!mTrainingData) {
        *mOutput << mName << ": could not allocate " << mTrainingDataSize << " doubles pointers for training data.\n";
        return false;
      }
    } else if (mTrainingSize >= mTrainingDataSize) {
      // training data vector too small
      double ** new_buf;
      mTrainingDataSize *= 2;
      new_buf = (double**)realloc(mTrainingData, mTrainingDataSize * sizeof(double*));
      if (!new_buf) {
        *mOutput << mName << ": could not allocate " << mTrainingDataSize << " doubles pointers for training data.\n";
        return false;
      } else mTrainingData = new_buf;
    }
    
    // 1. read mVectorSize doubles
    double * vector = (double*) malloc(mVectorSize * sizeof(double));
    if (!vector) {
      *mOutput << mName << ": could not allocate " << mVectorSize << " doubles for training vector " << mTrainingSize+1 << ".\n";
      return false;
    }
    float val;
    for(int i=0; i < mVectorSize; i++) {
      // read a float
      if(fscanf(mTrainFile, " %f", &val) <= 0) {
        *mOutput << mName << ": file format reading training vector " << mTrainingSize+1 << " error.\n";
        return false;
      }
      fscanf(mTrainFile, "\n"); // ignore newline
      vector[i] = (double)val;
    }
    
    mTrainingData[mTrainingSize] = vector;
    mTrainingSize++;
  }
  
  void free_buffer(double ** pBuffer, int pMajorSize)
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
  double ** mCodebook;     /**< Array of mCodebookSize pointers to vectors of size mVectorSize. */
  double ** mTrainingData; /**< Array of mTrainingSize pointers to vectors of size mVectorSize. */
  FILE *    mTrainFile;    /**< Where the training vectors are stored. */
  int       mTrainingDataSize;  /**< Size of mTrainingData buffer. */
  int       mTrainingSize;  /**< Number of training vectors. */
  int       mCodebookSize; /**< Number of prototypes. */
};

extern "C" void init()
{
  CLASS(Quantize)
  OUTLET(Quantize, label)
}