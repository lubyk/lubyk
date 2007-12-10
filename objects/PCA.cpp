#include "trained_machine.h"
#include "matrix.h"

#define INITIAL_CLASS_COUNT 32

/** Principal Component Analysis. Input matrix is considered as a single vector of size row x col. */
class PCA : public TrainedMachine
{
public:

  bool init(const Params& p)
  {
    if (!set_machine(p)) return false;
    mTransposedFolder = "processed"; // where to store training data transposed in new basis
    
    if (!set_size(mBuffer,     1,  8, "mBuffer")) return false;
    if (!set_size(mWorkBuffer, 1, 32, "mWorkBuffer")) return false;
    if (!set_size(mMeanValue,  1, 32, "mMeanValue" )) return false;
    if (!set_size(mBasis,      8, 32, "mMeanValue" )) return false;
    mS.set(mBuffer);
    load_model();
    return true;
  }

  bool set(const Params& p)
  {
    size_t input_size  = mMeanValue.col_count();
    size_t output_size = mBuffer.col_count();
    
    p.get(&mTransposedFolder, "processed");
    if (p.get(&output_size, "keep"))
      if (!set_sizes(mBuffer, 1, output_size, "mBuffer")) return false;
    
    if (p.get(&input_size, "vector")) {
      if (!set_sizes(mWorkBuffer, 1, input_size, "mWorkBuffer")) return false;
      if (!set_sizes(mMeanValue,  1, input_size, "mMeanValue" )) return false;
    }
    
    if (mBasis.row_count() != output_size || mBasis.col_count() != input_size) {
      if (!set_size(mBasis, output_size, input_size, "mBasis" )) return false;
    }
    return true;
  }
  
  // inlet 1
  void bang (const Signal& sig)
  {
    if (!mIsOK) return; // no recovery
    
    if (sig.type == MatrixSignal) {
      if (sig.matrix.value->size() != mMeanValue.size()) {
        *mOutput << mName << ": wrong signal size " << sig.matrix.value->size() << " should be " << mMeanValue.size() << "\n.";
        return;
      }
      sig.get(&mLiveBuffer);
      
      transpose_vector(mLiveBuffer);

      // send
      if (mDebug) std::cout << mName << ": " << mS << std::endl;
      send(mS);
      
    } else {
      *mOutput << mName << ": wrong signal type (" << sig.type_name() << ") should be MatrixSignal\n";
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
    *mOutput << mName << ": " << mBasis << std::endl;
  }
    
private:
  inline void transpose_vector(const Matrix& pMat)
  {
    // remove mean value
    if(!mWorkBuffer.add(pMat, mMeanValue, 1.0, -1.0)) return;

    // change vector basis ( S' = SP' ) mBasis = P
    mBuffer.mat_multiply(mWorkBuffer, mBasis, CblasNoTrans, CblasTrans);
    //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 1, mTargetSize, mVectorSize, 1, mWorkBuffer, mVectorSize, mBasis, mVectorSize, 0.0, mBuffer, mVectorSize);
  }
  
  bool compute_mean_vector(const std::string& filename, Matrix * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (mVectorCount > 0) {
        mMeanValue /= (double)mVectorCount;
        
        if(!mClasses.append(mMeanValue)) {
          *mOutput << mName << ": append class (" << mClasses.error_msg() << ").\n";
          return false;
        }
        
        if (filename != "")
          *mOutput << mName << ": read '" << filename << "' (" << mVectorCount << " vectors)\n";
      }
      
      mVectorCount = 0;
      mMeanValue.clear();
      return true;
    }
    
    mMeanValue += vector;
    
    mVectorCount++;
    return true;
  }
  
  bool transpose_in_new_basis(const std::string& filename, Matrix * vector)
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
    
    if(!vector.to_file(mTransposedFile)) {
      *mOutput << mName << ": write transposed vector (" << vector.error_msg() << ")";
      return false;
    }
    
    return true;
  }
  
  void load_model()
  { 
    Matrix vector;
    size_t target_size = mBuffer.col_count();
    
    if (!set_size(mBasis, 0, mMeanValue.col_count(), "mBasis" )) return;
    
    FILE * file = fopen(pca_model_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": could not read from '" << pca_model_path() << "' (" << strerror(errno) << ")\n";
        return;
      }
      // read mMeanValue vector
      if(!mMeanValue.from_file(file)) {
        *mOutput << mName << ": mMeanValue (" << mMeanValue.error_msg() << ").\n";
        return;
      }
      
      while(vector.from_file(file)) {
        if(!mBasis.append(vector)) {
          *mOutput << mName << ": mBasis append (" << mBasis.error_msg() << ").\n";
          return;
        }
        if (mBasis.row_count() > target_size ) {
          *mOutput << mName << ": wrong dimension of pca basis. Found " << mBasis.row_count() << "x" << mBasis.col_count() << " when matrix should be " << target_size << "x" << mMeanValue.col_count() << "\n";
          return;
        }
      }
    fclose(file);
  }
  
  bool do_learn()
  {
    Matrix symmetric_matrix;
    
    ///////// REWRITE TO HERE ///////////
    
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
    *mOutput << mName << ": calculated " << eigen_count << " eigenvalues (showing greater then zero):\n  ";
    
    for(int i= 0; i< eigen_count; i++) {
      if (eigenvalues[i] < -0.0001 || eigenvalues[i] > 0.0001) {
        bprint(mBuf,mBufSize, " % .4f", eigenvalues[i]);
        *mOutput << mBuf;
      }
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
        goto learn_failed;
      }
      // keep only the greatest eigenvectors with greatest eigenvalues.
      
      if (mBasis) free(mBasis);
      if (!alloc_doubles(&mBasis, mVectorSize * mTargetSize, "basis matrix")) goto learn_failed;
      
      double value;
      for(int e=0; e < mTargetSize; e++) {
        int eigen_id = eigen_count - e - 1; // start by greatest eigenvalue (last).
        for(int i=0; i< mVectorSize; i++) {
          value = eigenvectors[ eigen_id * mVectorSize + i ]; // eigenvalue is in column form, in CblasColMajor
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