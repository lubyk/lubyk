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
  
  bool init(const Value &p)
  {
    mTransposedFolder = "processed"; // where to store training data transposed in new basis
    mTransposedFile   = NULL;
    TRY(mBuffer,     set_sizes(1,8));
    TRY(mWorkBuffer, set_sizes(1,32));
    TRY(mMeanValue,  set_sizes(1,32));
    TRY(mBasis,      set_sizes(8,32));
    
    mS.set(mBuffer);
    return init_machine(p);
  }

  bool set(const Value &p)
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
    return set_machine(p);
  }
  
  // inlet 1
  void bang (const Value &val)
  {
    if (!is_ok_) return; // no recovery
    
    if (val.type == MatrixValue) {
      if (val.matrix.value->size() != mMeanValue.size()) {
        *output_ << name_ << ": wrong signal size " << val.matrix.value->size() << " should be " << mMeanValue.size() << "\n.";
        return;
      }
      val.get(&mLiveBuffer);
      
      transpose_vector(*mLiveBuffer);

      // send
      send(mS);
      
    } else {
      *output_ << name_ << ": wrong signal type (" << val.type_name() << ") should be MatrixValue\n";
      return;
    }
    
  }

  /** Command to print the current basis. */
  void basis()
  {
    *output_ << name_ << ": " << mBasis << std::endl;
  }
  
  virtual const Value inspect(const Value &val) 
  {  
    bprint(mSpy, mSpySize,"%ix%i", mBasis.col_count(), mBasis.row_count());    
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
  
  bool compute_mean_vector(const std::string &filename, Matrix * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (mVectorCount > 0) {
        mMeanValue /= (real_t)mVectorCount;
        
        TRY(class_es, append(mMeanValue));
        
        if (filename != "")
          *output_ << name_ << ": read '" << filename << "' (" << mVectorCount << " vectors)\n";
      }
      
      mVectorCount = 0;
      mMeanValue.clear();
      return true;
    }
    
    mMeanValue += *vector;
    
    mVectorCount++;
    return true;
  }
  
  bool transpose_in_new_basis(const std::string &filename, Matrix * vector)
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
        *output_ << name_ << ": could not open '" << pca_transpose_path(filename) << "' to write transposed data.\n";
        return false;
      }
    }
    
    // transpose vector
    transpose_vector(*vector);
    
    TRY(mBuffer, to_file(mTransposedFile));
    
    return true;
  }
  
  bool load_model()
  { 
    Matrix vector;
    size_t target_size = mBuffer.col_count();
    
    TRY(mBasis, set_sizes(0, mMeanValue.col_count()));
    
    FILE * file = fopen(pca_model_path().c_str(), "rb");
      if (!file) {
        *output_ << name_ << ": could not read from '" << pca_model_path() << "' (" << strerror(errno) << ")\n";
        return false;
      }
      // read mMeanValue vector
      TRY(mMeanValue, from_file(file));
      
      while(vector.from_file(file)) {
        TRY(mBasis, append(vector));
        
        if (mBasis.row_count() > target_size ) {
          *output_ << name_ << ": wrong dimension of pca basis. Found " << mBasis.row_count() << "x" << mBasis.col_count() << " when matrix should be " << target_size << "x" << mMeanValue.col_count() << "\n";
          fclose(file);
          return false;
        }
      }
    fclose(file);
    return true;
  }
  
  bool learn_from_data()
  {
    Matrix symmetric_matrix, eigenvectors, eigenvalues;
    
    // 1. build a matrix with the mean vectors for each class
    TRY(class_es, set_sizes(0, mMeanValue.col_count()));
    
    if(!FOREACH_TRAIN_CLASS(PCA, compute_mean_vector)) {
      *output_ << name_ << ": could not build model.\n";
      return false;
    }
    
    if (class_es.row_count() == 0) {
      *output_ << name_ << ": no class file.\n";
      return false;
    }
    
    // 2.1. compute mean value for all vectors
    mMeanValue.clear();
    
    for(size_t c=0; c < class_es.row_count(); c++)
      if (!mMeanValue.add(class_es, c, c)) return false;
    
    mMeanValue /= class_es.row_count();
    
    // 2.2 remove from each element
    TRY(class_es, subtract(mMeanValue));
    
    // write mean value to model file
    TRY(mMeanValue, to_file(pca_model_path(), "wb"));
    
    // we have our matrix.
    if (mDebug) *output_ << name_ << ": source:\n" << class_es << "\n";
    
    // 3. PCA
    
    // Compute T'T :
    TRY(symmetric_matrix, symmetric(class_es));
    
    if (mDebug) *output_ << name_ << ": symmetric:\n" << symmetric_matrix << "\n";
    
    // Find eigenvectors, eigenvalues of T'T :
    TRY(eigenvectors, eigenvectors(eigenvalues, symmetric_matrix));
    
    // build P out of those eigenvectors with greatest values :
    *output_ << name_ << ": calculated " << eigenvalues.col_count() << " eigenvalues (showing greater then zero):\n  ";
    
    for(size_t i= 0; i < eigenvalues.col_count(); i++)
      if (eigenvalues[0][i] < -0.0001 || eigenvalues[0][i] > 0.0001)
        *output_ << " " << eigenvalues[0][i];
        
    *output_ << "\n";
    
    if (mDebug) *output_ << name_ << ": eigenvectors:\n" << eigenvectors << "\n";
    
    // P = partial E'.
    
    // write P to file
    // build basis P
    TRY(mBasis, copy(eigenvectors, - mBuffer.col_count(), - 1)); // take last vectors (greatest eigenvalues)
    
    TRY(mBasis, to_file(pca_model_path(), "ab"));
    
    if (mDebug) {
      *output_ << name_ << ": basis P:\n" << mBasis << "\n";
    } else {
      *output_ << name_ << ": new basis with " << mBasis.row_count() << " dimensions from a vectors of size " << mBasis.col_count() << " computed.\n";
    }
    
    if(!FOREACH_TRAIN_CLASS(PCA, transpose_in_new_basis)) {
      *output_ << name_ << ": could not write transposed training data\n";
      return false;
    }
    
    *output_ << name_ << ": wrote transposed training data to '" << pca_transpose_path(std::string("")) << "'.\n"; 
    
    return true;
  }
  
  std::string pca_model_path()
  {
    std::string path(mFolder);
    path.append("/").append(name_).append(".model");
    return path;
  }

  std::string pca_transpose_path(const std::string &pFilename)
  {
    std::string path(mTransposedFolder);
    path.append("/").append(pFilename);
    return path;
  }
  
  
  
  std::string mTransposedFolder; /**< Where to store training data transposed in new basis. */

  const Matrix * mLiveBuffer;      /**< Live input signal. */
  Matrix   mWorkBuffer;      /**< Input, mean value removed. */
  Matrix   mBuffer;          /**< Output signal. */
  Matrix   class_es;         /**< Large array with all mean values for each class. */
  Matrix   mBasis;           /**< Matrix to change basis (stored in file xxx.model). Size is mTargetSize x mVectorSize */
  Matrix   mMeanValue;       /**< Mean value to be removed from vector before change of basis. */
  int      mVectorCount;     /**< Number of vectors (used during foreach class loop). */
  FILE *   mTransposedFile;  /**< Where to store the transposed data for a class (FILE *). */
  
};


extern "C" void init(Planet &planet) {
  CLASS (PCA)
  OUTLET(PCA,scaled)
  SUPER_METHOD(PCA, TrainedMachine, learn)
  SUPER_METHOD(PCA, TrainedMachine, load)
  METHOD(PCA,basis)
}