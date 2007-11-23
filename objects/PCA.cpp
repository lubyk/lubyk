
#include "class.h"
#include "matrix.h"
#include <errno.h>     // Error number definitions
#include <sys/types.h> // directory listing
#include <dirent.h>    // directory listing

#include <algorithm>
#include <vector>

#define INITIAL_CLASS_COUNT 32

struct EigenVector
{
  EigenVector(double pValue, int pId) : eigenvalue(pValue), eigen_id(pId) {}
  
  int eigen_id;
  double eigenvalue;
};


// descending sort
bool operator < (const EigenVector& left, const EigenVector& right)
{
	if  (left.eigenvalue < right.eigenvalue)
	  return false;
	else
    return true;
}

class PCA : public Node
{
public:
  virtual ~PCA ()
  {
    if (mBuffer)  free(mBuffer);
    if (mWorkBuffer)  free(mWorkBuffer);
    if (mClasses) free(mClasses);
    if (mMeanValue) free(mMeanValue);
  }

  bool init(const Params& p)
  {
    mFolder     = p.val("data", std::string("data"));
    mVectorSize = p.val("vector", 32);
    mUnitSize   = p.val("unit", 1);
    mTargetSize = p.val("keep", 4); // target dimension
    
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

        // change vector basis ( S' = SP = SP'') mBasis = P'
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
private:
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
    std::vector<EigenVector> eig_result_list;
    FILE   * file;
    
    // 1. build a matrix with the mean vectors for each class
    mClassCount = 0;
    
    DIR * directory;
    struct dirent *elem;
    double * vector;
    if (!alloc_doubles(&vector, mVectorSize, "work vector")) return;
    if (mMeanValue) free(mMeanValue);
    if (!alloc_doubles(&mMeanValue, mVectorSize, "mean value")) return;

    directory = opendir(mFolder.c_str());
      if (directory == NULL) {
        *mOutput << mName << ": could not open directory '" << mFolder << "' to read class data.\n";
        goto build_cleanup;
      }
      while (elem = readdir(directory)) {
        std::string filename(elem->d_name);
        std::string label;
        int start, end;
        start = filename.find("class_");
        end   = filename.find(".txt");
        if (start == std::string::npos || end == std::string::npos) {
          // bad filename, skip
          continue;
        }
        compute_mean_vector(&vector, std::string(mFolder).append("/").append(filename));
        if (!add_class(vector)) goto build_cleanup;
      }
    closedir(directory);
    
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
    Matrix::print_matrix(mClasses, mClassCount, mVectorSize, CblasRowMajor);
    printf("\n");
    
    // 3. PCA
    
    // Compute T'T :
    if(!alloc_doubles(&symmetric_matrix, mVectorSize * mVectorSize, "symmetric matrix")) goto build_cleanup;
    if(!Matrix::compute_symetric_matrix(&symmetric_matrix, mClasses, mClassCount, mVectorSize)) goto build_cleanup;
    
    
    Matrix::print_matrix(symmetric_matrix, mVectorSize, mVectorSize, CblasRowMajor);
    printf("\n");
    
    // Find eigenvectors, eigenvalues of T'T :
    // find the eigenvectors of T'T :
    long eigen_count;
    int * sort_index;
    if(!alloc_doubles(&eigenvectors, mVectorSize * mVectorSize, "eigenvectors matrix")) goto build_cleanup;
    if(!alloc_doubles(&eigenvalues,  mVectorSize, "eigenvalues")) goto build_cleanup;
    if(!Matrix::compute_eigenvectors(&eigenvectors, &eigenvalues, &eigen_count, symmetric_matrix, mVectorSize)) goto build_cleanup;

    
    
    // build P out of those eigenvectors with greatest values :
    *mOutput << mName << ": calculated " << eigen_count << " eigenvalues:";
    
    for(int i=0; i < eigen_count; i++) {
      bprint(mBuf,mBufSize, " %.3f", eigenvalues[i]);
      *mOutput << mBuf;
      eig_result_list.push_back(EigenVector(eigenvalues[i], i));
    }
    *mOutput << std::endl;
    
    // sort eigenvectors...
    std::sort(eig_result_list.begin(), eig_result_list.end());
    
    // P = partial E'.
    
    // write P' to file = partial E
    
    // EIGENVECTORS ARE COL MAJOR ?
    /** eigenvectors:
      *     | e11 e21 |
      *  E =| e12 e22 |
      *     | e13 e23 | **/
    file = fopen(pca_model_path().c_str(), "ab");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << pca_model_path() << "' (" << strerror(errno) << ")\n~> ";
        goto build_cleanup;
      }
      
      for(int e=0; e < mTargetSize; e++) {
        int eigen_id = eig_result_list[e].eigen_id; // position in result list
        for(int i=0; i< mVectorSize; i++) {
          fprintf(file, " % .5f", eigenvectors[ i * eigen_count + eigen_id ]); // ColMajor
          if (mUnitSize > 1 && (i+1)%mUnitSize == 0) fprintf(file, "\n");
        }  
        fprintf(file, "\n");  // two \n\n between vectors
      }
    fclose(file);
    
build_cleanup:
    if (symmetric_matrix)    free(symmetric_matrix);
    if (vector) free(vector);
    if (eigenvectors) free(eigenvectors);
    if (eigenvalues) free(eigenvalues);
    return;
  }
  
  void compute_mean_vector(double ** pMeanVector, const std::string& pPath)
  {
    int vector_count = 0;
    double * vector = *pMeanVector;
    for(int i=0; i < mVectorSize; i++) vector[i] = 0.0;
    
    FILE * file;
    float val;
    int    value_count = 0;
    file = fopen(pPath.c_str(), "rb");
      if (!file) {
        *mOutput << mName << "(error): could not read from '" << pPath << "' (" << strerror(errno) << ")\n";
        return;
      }
      // read a vector
      while(fscanf(file, " %f", &val) != EOF) {
        fscanf(file, "\n"); // ignore newline
        vector[value_count] += (double)val;
        value_count++;
        if (value_count >= mVectorSize) {
          // got one vector
          vector_count++;
          value_count = 0;
        }
      }
    fclose(file);
    if (vector_count > 0) {
      // compute mean value:
      for(int i=0; i < mVectorSize; i++) {
        vector[i] /= (double)vector_count;
      }
    }
    *mOutput << mName << ": read '" << pPath << "' (" << vector_count << " vectors)\n";
    // for(int i=0; i < mVectorSize; i++) {
    //   printf(" % .5f", vector[i]);
    //   if ((i+1) % 12 == 0) printf("\n");
    // }
  }
  
  /** Add vector set of classes. */
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

  
  std::string mFolder; /**< Folder containing the class data. */

  int      mClassCount;      /**< Number of different classes. */
  int      mVectorSize;      /**< Number of samples per vector. */
  double * mLiveBuffer;      /**< Live input signal. */
  double * mWorkBuffer;      /**< Input without mean value. */
  double * mBuffer;          /**< Output signal. */
  double * mClasses;         /**< Large array with all meanVectors. */
  double * mBasis;           /**< Matrix to change basis (stored in file xxx.pca). Size is mTargetSize x mVectorSize */
  double * mMeanValue;       /**< Mean value to be removed from vector before change of basis. */
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
}