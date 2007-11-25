#include "trained_machine.h"
#include "matrix.h"

#define INITIAL_CLASS_COUNT 32

/** Principal Component Analysis. */
class PCA : public TrainedMachine
{
public:
  virtual ~PCA ()
  {
    if (mBuffer)  free(mBuffer);            // FIXME: replace these buffers with Buf<double>
    if (mWorkBuffer)  free(mWorkBuffer);    // FIXME: replace these buffers with Buf<double>
    if (mClasses) free(mClasses);           // FIXME: replace these buffers with Buf<double>
    if (mMeanValue) free(mMeanValue);       // FIXME: replace these buffers with Buf<double>
  }

  bool init(const Params& p)
  {
    if (!init_machine(p)) return false;
    mProcessedFolder     = p.val("processed", std::string("processed")); // where to store training data transposed in new basis
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
        
        // remove mean value
        for(int i=0; i < mVectorSize; i++) {
          mWorkBuffer[i] = mLiveBuffer[i] - mMeanValue[i];
        }

        // change vector basis ( S' = SP ) mBasis = P
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 1, mTargetSize, mVectorSize, 1, mWorkBuffer, mVectorSize, mBasis, mVectorSize, 0.0, mBuffer, mVectorSize);

        // send
        mS.type = ArraySignal;
        mS.array.value = mBuffer;
        mS.array.size  = mTargetSize;
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
  void build()
  {
    build_model();
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
  
  /** Transpose training data in new basis space. */
  void transpose()
  {
    
  }
  
private:
  bool compute_mean_vector(const std::string& filename, double * vector)
  {
    if (vector == NULL) {
      // class finished // initialize
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
  
  void build_model()
  {
    double * symmetric_matrix = NULL;
    double * eigenvectors = NULL;
    double * eigenvalues  = NULL;
    FILE   * file;
    
    // 1. build a matrix with the mean vectors for each class
    mClassCount = 0;
    if (mClasses)   free(mClasses);
    if (mMeanValue) free(mMeanValue);
    if (!alloc_doubles(&mMeanValue, mVectorSize, "mean value")) return;

    if(!FOREACH_TRAIN_CLASS(PCA, compute_mean_vector)) {
      *mOutput << mName << ": could not build model\n";
      return;
    }
    
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
        goto build_cleanup;
      }
    
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
    if(!alloc_doubles(&symmetric_matrix, mVectorSize * mVectorSize, "symmetric matrix")) goto build_cleanup;
    if(!Matrix::compute_symetric_matrix(&symmetric_matrix, mClasses, mClassCount, mVectorSize)) goto build_cleanup;
    
    
    if (mDebug) {
      *mOutput << mName << ": symmetric:\n";
      Matrix::print(symmetric_matrix, mVectorSize, mVectorSize, CblasRowMajor);
      printf("\n");
    }
    // Find eigenvectors, eigenvalues of T'T :
    // find the eigenvectors of T'T :
    long eigen_count;
    int * sort_index;
    if(!alloc_doubles(&eigenvectors, mVectorSize * mVectorSize, "eigenvectors matrix")) goto build_cleanup;
    if(!alloc_doubles(&eigenvalues,  mVectorSize, "eigenvalues")) goto build_cleanup;
    if(!Matrix::compute_eigenvectors(&eigenvectors, &eigenvalues, &eigen_count, symmetric_matrix, mVectorSize)) goto build_cleanup;

    
    
    // build P out of those eigenvectors with greatest values :
    *mOutput << mName << ": calculated " << eigen_count << " eigenvalues:";
    
    for(int i= 0; i< eigen_count; i++) {
      bprint(mBuf,mBufSize, " %.3f", eigenvalues[i]);
      *mOutput << mBuf;
    }
    *mOutput << std::endl;
    
    if (mDebug) {
      *mOutput << mName << ": eigenvectors:\n";
      Matrix::print(eigenvectors, mVectorSize, mVectorSize, CblasColMajor);
      printf("\n");
    }
    
    // P = partial E'.
    
    // write P to file
    file = fopen(pca_model_path().c_str(), "ab");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << pca_model_path() << "' (" << strerror(errno) << ")\n~> ";
        goto build_cleanup;
      }
      // keep only the greatest eigenvectors with greatest eigenvalues.
      
      if (mBasis) free(mBasis);
      if (!alloc_doubles(&mBasis, mVectorSize * mTargetSize, "basis matrix")) goto build_cleanup;
      
      double value;
      for(int e=0; e < mTargetSize; e++) {
        int eigen_id = eigen_count - e - 1; // start by greatest eigenvalue (last).
        for(int i=0; i< mVectorSize; i++) {
          value = eigenvectors[ i * eigen_count + eigen_id ]; // transpose to RowMajor
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
    
build_cleanup:
    if (symmetric_matrix)    free(symmetric_matrix);
    if (eigenvectors) free(eigenvectors);
    if (eigenvalues) free(eigenvalues);
    return;
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
    path.append("/").append(mName).append(".pca");
    return path;
  }
  
  
  
  std::string mProcessedFolder; /**< Where to store training data transposed in new basis. */

  int      mClassCount;      /**< Number of different classes. */
  double * mLiveBuffer;      /**< Live input signal. */
  double * mWorkBuffer;      /**< Input without mean value. */
  double * mBuffer;          /**< Output signal. */
  double * mClasses;         /**< Large array with all meanVectors. */
  double * mBasis;           /**< Matrix to change basis (stored in file xxx.pca). Size is mTargetSize x mVectorSize */
  double * mMeanValue;       /**< Mean value to be removed from vector before change of basis. */
  int      mVectorCount;     /**< Number of vectors (used during foreach class loop). */
  int      mClassesSize;     /**< Size of array of meanVectors. */
  int      mLiveBufferSize;  /**< Size of live buffer. */
  int      mTargetSize;
  int      mUnitSize;
  
};


extern "C" void init()
{
  CLASS (PCA)
  OUTLET(PCA,scaled)
  METHOD(PCA,build)
  METHOD(PCA,load)
  METHOD(PCA,basis)
}