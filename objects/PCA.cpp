#include "trained_machine.h"
#include "matrix.h"

#define INITIAL_CLASS_COUNT 32

/** Principal Component Analysis. */
class PCA : public TrainedMachine
{
public:
  PCA() : mBuffer(NULL), mWorkBuffer(NULL), mClasses(NULL), mMeanValue(NULL), mTransposedFile(NULL) {}
  
  virtual ~PCA ()
  {
    if (mBuffer)  free(mBuffer);            // FIXME: replace these buffers with Buf<double>
    if (mWorkBuffer)  free(mWorkBuffer);    // FIXME: replace these buffers with Buf<double>
    if (mClasses) free(mClasses);           // FIXME: replace these buffers with Buf<double>
    if (mMeanValue) free(mMeanValue);       // FIXME: replace these buffers with Buf<double>
    if (mTransposedFile) fclose(mTransposedFile);
  }

  bool init(const Params& p)
  {
    if (!init_machine(p)) return false;
    mTransposedFolder     = p.val("processed", std::string("processed")); // where to store training data transposed in new basis
    mUnitSize   = p.val("unit", 1);
    mTargetSize = p.val("keep", 4); // target dimension
    if (mTargetSize > mVectorSize) {
      *mOutput << mName << ": cannot keep more dimensions (" << mTargetSize << ") then vector size (" << mVectorSize << ").\n";
      mTargetSize = mVectorSize;
    }
    
    
    if(!alloc_doubles(&mBuffer, mVectorSize, "output stream")) return false;
    if(!alloc_doubles(&mWorkBuffer, mVectorSize, "work buffer")) return false;
    load_model();
    return true;
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    if (!mIsOK) return; // no recovery
    
    if (sig.type == ArraySignal) {
      if (sig.array.size >= mVectorSize) {
        mLiveBuffer     = sig.array.value;
        mLiveBufferSize = sig.array.size;
        
        transpose_vector(mLiveBuffer);

        // send
        mS.type = ArraySignal;
        mS.array.value = mBuffer;
        mS.array.size  = mTargetSize;
        if (mDebug)
          std::cout << mName << ": " << mS << std::endl;
        send(mS);
      } else {
        *mOutput << mName << ": wrong signal size " << sig.array.size << " should be " << mVectorSize << "\n.";
      }
    } else {
      *mOutput << mName << ": wrong signal type (" << sig.type_name() << ") should be ArraySignal\n";
      return;
    }
    
  }
  
  /** Method from command line. */
  void learn()
  {
    if(!do_learn()) {
      *mOutput << mName << ": could not create a new basis from training data.\n";
    }
  }
  
  void load()
  {
    load_model();
  }

  /** Command to print the current basis. */
  void basis()
  {
    if (!mBasis) {
      *mOutput << mName << ": basis not computed.\n";
      return;
    }
    Matrix::print(mBasis, mTargetSize, mVectorSize, CblasRowMajor);
  }
    
private:
  inline void transpose_vector(double * vector)
  {
    // remove mean value
    for(int i=0; i < mVectorSize; i++) {
      mWorkBuffer[i] = vector[i] - mMeanValue[i];
    }

    // change vector basis ( S' = SP' ) mBasis = P
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 1, mTargetSize, mVectorSize, 1, mWorkBuffer, mVectorSize, mBasis, mVectorSize, 0.0, mBuffer, mVectorSize);
  }
  
  bool compute_mean_vector(const std::string& filename, double * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (mVectorCount > 0) {
        for(int i=0; i < mVectorSize; i++)
          mMeanValue[i] /= (double)mVectorCount;
        
        if(!add_class(mMeanValue)) return false;
        if (filename != "")
          *mOutput << mName << ": read '" << filename << "' (" << mVectorCount << " vectors)\n";
      }
      
      mVectorCount = 0;
      for(int i=0; i < mVectorSize; i++)
        mMeanValue[i] = 0.0;
      return true;
    }
    
    for(int i=0; i < mVectorSize; i++)
      mMeanValue[i] += vector[i];
    
    mVectorCount++;
    return true;
  }
  
  bool transpose_in_new_basis(const std::string& filename, double * vector)
  {
    if (vector == NULL) {
      // initialize or finished a class
      if (mTransposedFile) fclose(mTransposedFile);
      mTransposedFile = NULL;
      return true;
    }
    if (!mTransposedFile) {
      mTransposedFile = fopen(pca_transpose_path(filename).c_str(), "wb");
      if (!mTransposedFile) {
        *mOutput << mName << ": could not open '" << pca_transpose_path(filename) << "' to write transposed data.\n";
        return false;
      }
    }
    // transpose vector
    transpose_vector(vector);
    
    // FIXME: write vector should be in 'Matrix'
    for(int i=0; i< mTargetSize; i++) {
      fprintf(mTransposedFile, " % .5f", mBuffer[i]);
      if (mUnitSize > 1 && (i+1)%mUnitSize == 0) fprintf(mTransposedFile, "\n");
    }
    fprintf(mTransposedFile, "\n\n");
    return true;
  }
  
  void load_model()
  { 
    float val;
    int vector_count = 0;
    int value_count  = 0;
    if (mMeanValue) free(mMeanValue);
    if (!alloc_doubles(&mMeanValue, mVectorSize, "mean value")) return;
    
    if (mBasis) free(mBasis);
    if (!alloc_doubles(&mBasis, mVectorSize * mTargetSize, "basis matrix")) return;
    
    FILE * file = fopen(pca_model_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << "(error): could not read from '" << pca_model_path() << "' (" << strerror(errno) << ")\n";
        return;
      }
      // read mMeanValue vector
      while(fscanf(file, " %f", &val) != EOF) {
        fscanf(file, "\n"); // ignore newline
        if(vector_count == 0) {
          mMeanValue[value_count] = (double)val;
        } else {
          if ((vector_count-1) >= mTargetSize || value_count >= mVectorSize ) {
            *mOutput << mName << ": wrong dimension of pca basis.\n";
            free(mBasis);
            mBasis = NULL;
            return;
          }
          mBasis[(vector_count - 1) * mVectorSize + value_count] = (double)val;
        }
        value_count++;
        if (value_count >= mVectorSize) {
          // got one vector
          vector_count++;
          value_count = 0;
        }
      }
    fclose(file);
  }
  
  bool do_learn()
  {
    double * symmetric_matrix = NULL;
    double * eigenvectors = NULL;
    double * eigenvalues  = NULL;
    FILE   * file;

    // 1. build a matrix with the mean vectors for each class
    mClassCount = 0;
    if (mClasses) {
      free(mClasses);
      mClasses = NULL;
    }
    if (mMeanValue) free(mMeanValue);
    if (!alloc_doubles(&mMeanValue, mVectorSize, "mean value")) return false;

    if(!FOREACH_TRAIN_CLASS(PCA, compute_mean_vector)) {
      *mOutput << mName << ": could not build model.\n";
      return false;
    }
    
    if (!mClasses) return false; // no classes loaded.
    
    // 2.1. compute mean value for all vectors
    for(int i=0; i< mVectorSize; i++) mMeanValue[i] = 0.0;
    
    for(int c=0; c < mClassCount; c++) {
      for(int i=0; i < mVectorSize; i++) {
        mMeanValue[i] += mClasses[c * mVectorSize + i];
      }
    }
    
    for(int i=0; i < mVectorSize; i++) {
      mMeanValue[i] /= (double)mClassCount;
    }
    
    // 2.2 remove from each element
    for(int c=0; c < mClassCount; c++) {
      for(int i=0; i < mVectorSize; i++) {
        mClasses[c * mVectorSize + i] -= mMeanValue[i];
      }
    }
    
    // write mean value to model file
    
    file = fopen(pca_model_path().c_str(), "wb");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << pca_model_path() << "' (" << strerror(errno) << ")\n~> ";
        goto learn_failed;
      }
    
      // FIXME: write vector should be in 'Matrix'
      for(int i=0; i< mVectorSize; i++) {
        fprintf(file, " % .5f", mMeanValue[i]);
        if (mUnitSize > 1 && (i+1)%mUnitSize == 0) fprintf(file, "\n");
      }  
      fprintf(file, "\n\n");  // two \n\n between vectors
    fclose(file);
    
    // we have our matrix.
    if (mDebug) {
      *mOutput << mName << ": source:\n";
      Matrix::print(mClasses, mClassCount, mVectorSize, CblasRowMajor);
      printf("\n");
    }
    // 3. PCA
    
    // Compute T'T :
    if(!alloc_doubles(&symmetric_matrix, mVectorSize * mVectorSize, "symmetric matrix")) goto learn_failed;
    if(!Matrix::compute_symetric_matrix(&symmetric_matrix, mClasses, mClassCount, mVectorSize)) goto learn_failed;
    
    
    if (mDebug) {
      *mOutput << mName << ": symmetric:\n";
      Matrix::print(symmetric_matrix, mVectorSize, mVectorSize, CblasRowMajor);
      printf("\n");
    }
    // Find eigenvectors, eigenvalues of T'T :
    // find the eigenvectors of T'T :
    long eigen_count;
    int * sort_index;
    if(!alloc_doubles(&eigenvectors, mVectorSize * mVectorSize, "eigenvectors matrix")) goto learn_failed;
    if(!alloc_doubles(&eigenvalues,  mVectorSize, "eigenvalues")) goto learn_failed;
    if(!Matrix::compute_eigenvectors(&eigenvectors, &eigenvalues, &eigen_count, symmetric_matrix, mVectorSize)) goto learn_failed;

    
    
    // build P out of those eigenvectors with greatest values :
    *mOutput << mName << ": calculated " << eigen_count << " eigenvalues:";
    
    for(int i= 0; i< eigen_count; i++) {
      bprint(mBuf,mBufSize, " % .7f\n", eigenvalues[i]);
      *mOutput << mBuf;
    }
    *mOutput << std::endl;
    
    if (mDebug) {
      *mOutput << mName << ": eigenvectors:\n";
      Matrix::print(eigenvectors, mVectorSize, mVectorSize, CblasRowMajor);
      printf("\n");
    }
    
    // P = partial E'.
    
    // write P to file
    file = fopen(pca_model_path().c_str(), "ab");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << pca_model_path() << "' (" << strerror(errno) << ")\n~> ";
        goto learn_failed;
      }
      // keep only the greatest eigenvectors with greatest eigenvalues.
      
      if (mBasis) free(mBasis);
      if (!alloc_doubles(&mBasis, mVectorSize * mTargetSize, "basis matrix")) goto learn_failed;
      
      double value;
      for(int e=0; e < mTargetSize; e++) {
        int eigen_id = eigen_count - e - 1; // start by greatest eigenvalue (last).
        for(int i=0; i< mVectorSize; i++) {
          value = eigenvectors[ eigen_id * mVectorSize + i ];
          fprintf(file, " % .5f", value);
          mBasis[e * mVectorSize + i] = value;
          if (mUnitSize > 1 && (i+1)%mUnitSize == 0) fprintf(file, "\n");
        }  
        fprintf(file, "\n");  // two \n\n between vectors
      }
    fclose(file);
    
    if (mDebug) {
      *mOutput << mName << ": basis P:\n";
      Matrix::print(mBasis, mTargetSize, mVectorSize, CblasRowMajor);
    } else {
      *mOutput << mName << ": new basis with " << mTargetSize << " dimensions from a vectors of size " << mVectorSize << " computed.\n";
    }
    
    if(!FOREACH_TRAIN_CLASS(PCA, transpose_in_new_basis)) {
      *mOutput << mName << ": could not write transposed training data\n";
      goto learn_failed;
    }
    
    *mOutput << mName << ": wrote transposed training data to '" << pca_transpose_path(std::string("")) << "'.\n"; 
    
    if (symmetric_matrix)    free(symmetric_matrix);
    if (eigenvectors) free(eigenvectors);
    if (eigenvalues) free(eigenvalues);
    return true;
learn_failed:
    if (symmetric_matrix)    free(symmetric_matrix);
    if (eigenvectors) free(eigenvectors);
    if (eigenvalues) free(eigenvalues);
    return false;
  }
    
  /** Add vector set of classes. */
  // FIXME: Use 'Buf' or 'Buf+Matrix' merge.
  bool add_class(double * vector)
  {
    if (!mClasses) {
      mClassesSize = INITIAL_CLASS_COUNT * mVectorSize;
      if (!alloc_doubles(&mClasses, mClassesSize, "class matrix")) return false;
    } else if (mClassCount + 1 >= mClassesSize) {
      // realloc
      mClassesSize *= 2;
      if (!realloc_doubles(&mClasses, mClassesSize, "class matrix")) return false;
    }
    for(int i=0; i < mVectorSize; i++)
      mClasses[mClassCount * mVectorSize + i] = vector[i];
      
    mClassCount++;
    return true;
  }
  
  std::string pca_model_path()
  {
    std::string path(mFolder);
    path.append("/").append(mName).append(".model");
    return path;
  }

  std::string pca_transpose_path(const std::string& pFilename)
  {
    std::string path(mTransposedFolder);
    path.append("/").append(pFilename);
    return path;
  }
  
  
  
  std::string mTransposedFolder; /**< Where to store training data transposed in new basis. */

  int      mClassCount;      /**< Number of different classes. */
  double * mLiveBuffer;      /**< Live input signal. */
  double * mWorkBuffer;      /**< Input without mean value. */
  double * mBuffer;          /**< Output signal. */
  double * mClasses;         /**< Large array with all meanVectors. */
  double * mBasis;           /**< Matrix to change basis (stored in file xxx.model). Size is mTargetSize x mVectorSize */
  double * mMeanValue;       /**< Mean value to be removed from vector before change of basis. */
  int      mVectorCount;     /**< Number of vectors (used during foreach class loop). */
  int      mClassesSize;     /**< Size of array of meanVectors. */
  int      mLiveBufferSize;  /**< Size of live buffer. */
  FILE *   mTransposedFile;  /**< Where to store the transposed data for a class (FILE *). */
  int      mTargetSize;
  int      mUnitSize;
  
};


extern "C" void init()
{
  CLASS (PCA)
  OUTLET(PCA,scaled)
  METHOD(PCA,learn)
  METHOD(PCA,load)
  METHOD(PCA,basis)
}