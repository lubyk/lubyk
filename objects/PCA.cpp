#include "trained_machine.h"
#include "matrix.h"

#define INITIAL_CLASS_COUNT 32

/** Principal Component Analysis. Input matrix is considered as a single vector of size row x col. */
class PCA : public TrainedMachine
{
public:

  ~PCA()
  {
    if (mTransposedFile) fclose(mTransposedFile);
  }
  
  bool init(const Params& p)
  {
    if (!set_machine(p)) return false;
    mTransposedFolder = "processed"; // where to store training data transposed in new basis
    
    TRY(mBuffer,     set_sizes(1,8));
    TRY(mWorkBuffer, set_sizes(1,32));
    TRY(mMeanValue,  set_sizes(1,32));
    TRY(mBasis,      set_sizes(8,32));
    
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
      TRY(mBuffer, set_sizes(1, output_size));
    
    if (p.get(&input_size, "vector")) {
      TRY(mWorkBuffer, set_sizes(1, input_size));
      TRY(mMeanValue,  set_sizes(1, input_size));
    }
    
    if (mBasis.row_count() != output_size || mBasis.col_count() != input_size) {
      TRY(mBasis, set_sizes(output_size, input_size));
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
      
      transpose_vector(*mLiveBuffer);

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
    if(!do_learn())
      *mOutput << mName << ": could not create a new basis from training data.\n";
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
    TRY_RET(mWorkBuffer, add(pMat, mMeanValue, 1.0, -1.0));

    // change vector basis ( S' = SP' ) mBasis = P
    TRY_RET(mBuffer, mat_multiply(mWorkBuffer, mBasis, CblasNoTrans, CblasTrans));
    //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 1, mTargetSize, mVectorSize, 1, mWorkBuffer, mVectorSize, mBasis, mVectorSize, 0.0, mBuffer, mVectorSize);
  }
  
  bool compute_mean_vector(const std::string& filename, Matrix * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (mVectorCount > 0) {
        mMeanValue /= (double)mVectorCount;
        
        TRY(mClasses, append(mMeanValue));
        
        if (filename != "")
          *mOutput << mName << ": read '" << filename << "' (" << mVectorCount << " vectors)\n";
      }
      
      mVectorCount = 0;
      mMeanValue.clear();
      return true;
    }
    
    mMeanValue += *vector;
    
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
    transpose_vector(*vector);
    
    TRY(mBuffer, to_file(mTransposedFile));
    
    return true;
  }
  
  void load_model()
  { 
    Matrix vector;
    size_t target_size = mBuffer.col_count();
    
    TRY_RET(mBasis, set_sizes(0, mMeanValue.col_count()));
    
    FILE * file = fopen(pca_model_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": could not read from '" << pca_model_path() << "' (" << strerror(errno) << ")\n";
        return;
      }
      // read mMeanValue vector
      TRY_RET(mMeanValue, from_file(file));
      
      while(vector.from_file(file)) {
        TRY_RET(mBasis, append(vector));
        
        if (mBasis.row_count() > target_size ) {
          *mOutput << mName << ": wrong dimension of pca basis. Found " << mBasis.row_count() << "x" << mBasis.col_count() << " when matrix should be " << target_size << "x" << mMeanValue.col_count() << "\n";
          return;
        }
      }
    fclose(file);
  }
  
  bool do_learn()
  {
    Matrix symmetric_matrix, eigenvectors, eigenvalues;
    
    // 1. build a matrix with the mean vectors for each class
    TRY(mClasses, set_sizes(0, mMeanValue.col_count()));
    
    if(!FOREACH_TRAIN_CLASS(PCA, compute_mean_vector)) {
      *mOutput << mName << ": could not build model.\n";
      return false;
    }
    
    if (mClasses.row_count() == 0) {
      *mOutput << mName << ": no class file.\n";
      return false;
    }
    
    // 2.1. compute mean value for all vectors
    mMeanValue.clear();
    
    for(size_t c=0; c < mClasses.row_count(); c++)
      if (!mMeanValue.add(mClasses, c, c)) return false;
    
    mMeanValue /= mClasses.row_count();
    
    // 2.2 remove from each element
    TRY(mClasses, subtract(mMeanValue));
    
    // write mean value to model file
    TRY(mMeanValue, to_file(pca_model_path(), "wb"));
    
    // we have our matrix.
    if (mDebug) *mOutput << mName << ": source:\n" << mClasses << "\n";
    
    // 3. PCA
    
    // Compute T'T :
    TRY(symmetric_matrix, symmetric(mClasses));
    
    if (mDebug) *mOutput << mName << ": symmetric:\n" << symmetric_matrix << "\n";
    
    // Find eigenvectors, eigenvalues of T'T :
    TRY(eigenvectors, eigenvectors(eigenvalues, symmetric_matrix));
    
    // build P out of those eigenvectors with greatest values :
    *mOutput << mName << ": calculated " << eigenvalues.col_count() << " eigenvalues (showing greater then zero):\n  ";
    
    for(size_t i= 0; i < eigenvalues.col_count(); i++)
      if (eigenvalues[0][i] < -0.0001 || eigenvalues[0][i] > 0.0001)
        *mOutput << " " << eigenvalues[0][i];
        
    *mOutput << "\n";
    
    if (mDebug) *mOutput << mName << ": eigenvectors:\n" << eigenvectors << "\n";
    
    // P = partial E'.
    
    // write P to file
    // build basis P
    TRY(mBasis, copy(eigenvectors, - mBuffer.col_count(), - 1)); // take last vectors (greatest eigenvalues)
    
    TRY(mBasis, to_file(pca_model_path(), "ab"));
    
    if (mDebug) {
      *mOutput << mName << ": basis P:\n" << mBasis << "\n";
    } else {
      *mOutput << mName << ": new basis with " << mBasis.row_count() << " dimensions from a vectors of size " << mBasis.col_count() << " computed.\n";
    }
    
    if(!FOREACH_TRAIN_CLASS(PCA, transpose_in_new_basis)) {
      *mOutput << mName << ": could not write transposed training data\n";
      return false;
    }
    
    *mOutput << mName << ": wrote transposed training data to '" << pca_transpose_path(std::string("")) << "'.\n"; 
    
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

  const Matrix * mLiveBuffer;      /**< Live input signal. */
  Matrix   mWorkBuffer;      /**< Input, mean value removed. */
  Matrix   mBuffer;          /**< Output signal. */
  Matrix   mClasses;         /**< Large array with all mean values for each class. */
  Matrix   mBasis;           /**< Matrix to change basis (stored in file xxx.model). Size is mTargetSize x mVectorSize */
  Matrix   mMeanValue;       /**< Mean value to be removed from vector before change of basis. */
  int      mVectorCount;     /**< Number of vectors (used during foreach class loop). */
  FILE *   mTransposedFile;  /**< Where to store the transposed data for a class (FILE *). */
  
};


extern "C" void init()
{
  CLASS (PCA)
  OUTLET(PCA,scaled)
  METHOD(PCA,learn)
  METHOD(PCA,load)
  METHOD(PCA,basis)
}