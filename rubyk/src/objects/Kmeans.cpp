#include "trained_machine.h"
#include <float.h> // DBL_MAX

/** Distance calculation algorithms. */
enum kmeans_distance_types_t {
  EuclideanDistance,        /**< Use distance to mean value. */
  EuclideanClosestDistance, /**< Use distance to closest sample point. */
  MahalanobisDistance,      /**< Distance to mean value divided by size of sample data ellipsoid in the direction of the distance. */
};

class Kmeans : public TrainedMachine
{
public:
  ~Kmeans()
  {
    clear_icov();
  }
  
  bool init(const Params& p)
  {
    mCodeBook.set_sizes(0,8); // arbitrary: 0 classes of 8 values
    mDistances.set_sizes(1,8);
    mDistanceThreshold = DBL_MAX;
    mDistanceType = EuclideanDistance;
    return true;
  }
  
  bool set(const Params& p)
  {
    size_t vector_size = mVector.col_count();
    if (p.get(&vector_size, "vector")) {
      TRY(mCodeBook, set_sizes(0,vector_size));
    }
    std::string dist;
    if (p.get(&dist, "distance")) {
      if (dist == "Mahalanobis")
        mDistanceType = MahalanobisDistance;
      else if (dist == "Closest")
        mDistanceType = EuclideanClosestDistance;
      else
        mDistanceType = EuclideanDistance;
    }
    
    p.get(&mDistanceThreshold, "threshold");
    
    return set_machine(p);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    const Matrix * live;
    if (sig.get(&live)) {
      // we automatically flatten (size()) matrix to vector.
      if (live->size() != mCodeBook.col_count()) {
        *mOutput << mName << ": bad input matrix " << live->row_count() << "x" << live->col_count() << " should be 1x" << mCodeBook.col_count() << ".\n";
        return;
      }
      mView.set_data(live->data); // flatten matrix to vector (FIXME: do not know if this is useful, maybe we should just break if the input is not a vector)
      if (get_label(mView)) {
        label_vector_from(mView, mLabel);
        send(4, mFullTrainingData);
        send(3, mLabelVector); // input value with current label
        send(2, mDistance);
        send(mLabel);
      } else {
        label_vector_from(mView, -1); // out of distance threshold
        send(4, mFullTrainingData);
        send(3, mLabelVector);
        if (mDebug) send(2, mDistance);
      }
    }
  }
  
  void probabilities()
  {
    *mOutput << mDistances << std::endl;
  }
  
  /**< Send out training data points. */
  void plot()
  {
    send(4, mFullTrainingData);
  }
private:
  
  bool get_label(const Matrix& live)
  {
    real_t closest_distance = DBL_MAX;
    real_t total_distance   = 0;
    size_t row_count = mCodeBook.row_count();
    size_t col_count = mCodeBook.col_count();
    mDistances.clear();
    int closest_label = 0;
    
    if (mDistanceType == EuclideanClosestDistance) {
      row_count = mFullTrainingData.row_count();
      col_count = mFullTrainingData.col_count();
      for (size_t i = 0; i < row_count; i++) {
        real_t d = 0.0;
        for (size_t j = 1; j < col_count; j++)
          d += (mFullTrainingData.data[i * col_count + j] - live.data[j-1]) * (mFullTrainingData.data[i * col_count + j] - live.data[j-1]);
        if (d < closest_distance) {
          closest_label = mFullTrainingData.data[i * col_count];
          closest_distance = d;
          // FIXME: set mDistances[i], find i from label.
        }
      }
      mDistance = closest_distance;
    } else {
      // Distances to mean value (Mahalanobis or Euclidean)
      for (size_t i = 0; i < row_count; i++) {
        real_t d = 0.0;
        if (mDistanceType == EuclideanDistance) {
          for (size_t j = 0; j < col_count; j++)
            d += (mCodeBook.data[i * col_count + j] - live.data[j]) * (mCodeBook.data[i * col_count + j] - live.data[j]);
        } else {
          // Mahalanobis distance                                           % Octave source code
          mWork1.copy(live); //                                             t  = live    
          mWork1.subtract(mCodeBook, i, i); // remove mean value            tc = t - M
          mWork2.mat_multiply(mWork1, *mICov[i]); //                        d  = tc * C
          mWork3.mat_multiply(mWork2, mWork1, CblasNoTrans, CblasTrans); // d  = d * tc'
          d = mWork3.data[0];               // square of distance
          //d = fast_sqrt(mWork3.data[0]);  // real distance
        }
        mDistances.data[i] = d;
        total_distance    += d;
        if (d < 0) {
          *mOutput << mName << ": error, corrupt covariance matrix for label '" << (char)mLabels.data[i] << "' (negative distance).\n";
        } else if (d < closest_distance) {
          closest_label = mLabels.data[i];
          closest_distance = d;
        }
      }
      // compute normalized probabilities:
      // not a good idea. Without normalization, 'on the spot' is 100% but 'in the middle' is (n-1)/n
      // probabilities change when the number of classes change. Distances are a safer indicator of the accuracy.
      //
      // if (row_count > 1)
      //   for (size_t i = 0; i < row_count; i++)
      //     mDistances.data[i] = (total_distance - mDistances.data[i]) / ((row_count - 1) * total_distance);
    
      mDistance = closest_distance;
    }
    if (mDistanceThreshold == 0 || mDistance < mDistanceThreshold) {
      mLabel = closest_label;
      return true;
    }
    return false;
  }
  
  bool load_model()
  {
    TRY(mLabels, set_sizes(1,0));  // 1 row, 0 = get column count from first row
    TRY(mCodeBook, set_sizes(0,0)); // read all until \n\n
    clear_icov();
    
    FILE * file = fopen(model_file_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": could not read model from '" << model_file_path() << "'.\n";
        return false;
      }
      TRY_OR(mLabels,   from_file(file), goto load_model_failed);
      TRY_OR(mCodeBook, from_file(file), goto load_model_failed);
      Matrix * tmp;
      for (size_t i = 0; i < mCodeBook.row_count(); i++) {
        tmp = new Matrix;
        TRY_OR((*tmp), from_file(file), goto load_model_failed);
        mICov.push_back(tmp);
      }
    fclose(file);
    
    TRY(mDistances, set_sizes(1,mCodeBook.row_count()));
    
    TRY(mWork1, set_sizes(1, mCodeBook.col_count()));
    TRY(mWork2, set_sizes(1, mCodeBook.col_count()));
    TRY(mWork3, set_sizes(1, 1));
    TRY(mView, set_sizes(1, mCodeBook.col_count()));
    
    
    TRY(mLabelVector, set_sizes(1, mCodeBook.col_count() + 1));
    TRY(mFullTrainingData, set_sizes(0, mCodeBook.col_count() + 1));
    
    if(!FOREACH_TRAIN_CLASS(Kmeans, load_training_samples)) {
      *mOutput << mName << ": could not load training data (to plot points).\n";
    }
    
    return true;
    
    load_model_failed:
    fclose(file);
    return false;
  }
  
  bool learn_from_data()
  {
    TRY(mCodeBook, set_sizes(0, mVector.col_count()));
    clear_icov();
    TRY(mMeanValue, set_sizes(1, mVector.col_count()));
    TRY(mLabels,   set_sizes(1,0));
    
    mVectorCount = 0;
    if(!FOREACH_TRAIN_CLASS(Kmeans, train_from_samples)) {
      *mOutput << mName << ": could not build model.\n";
      return false;
    }
    
    *mOutput << mName << ": built codebook of size " << mCodeBook.row_count() << "x" << mCodeBook.col_count() << ".\n";
    *mOutput << mName << ": labels = " << mLabels << ".\n";
    TRY(mLabels,   to_file(model_file_path()));
    TRY(mCodeBook, to_file(model_file_path(), "ab"));
    
    for (size_t i = 0; i < mICov.size(); i++) {
      TRY((*(mICov[i])), to_file(model_file_path(), "ab"));
    }
    return true;
  }
  
  bool train_from_samples(const std::string& pFilename, Matrix * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (mVectorCount == 0) {
        // initialize
        int label;
        if (!get_label_from_filename(&label, pFilename)) return false;
        mLabels.append(label);
      } else {
        // finish
        mMeanValue /= (real_t)mVectorCount;
        
        TRY(mCodeBook, append(mMeanValue));
        
        Matrix * tmp = new Matrix;
        if (mTrainingSet.row_count() > 1) {
          // compute C-1                                % Octave source code
          //                                            [mx,nx] = size(X)
          //                                            M = sum(X,1)/mx     % mean value
          
          // 1. remove mean value                       Xc = X - M(ones(mx,1),:)
          TRY(mTrainingSet, subtract(mMeanValue));
        
          // 2. compute S'S                             C  = (Xc' * Xc) / (mx - 1)
          TRY((*tmp), symmetric(mTrainingSet));
          *tmp /= (mTrainingSet.row_count() - 1);
          // 3. find inverse of covariance matrix tmp   C = inv(C)
          if (!tmp->inverse()) {
            *mOutput << mName << ": warning. Not enough training data (" << mVectorCount << ") to build covariance matrix for class '" << pFilename << "'. Using identity matix.\n";
            TRY((*tmp), identity(tmp->col_count()));
          }
        } else {
          *mOutput << mName << ": warning. Not enough training data (" << mVectorCount << ") to build covariance matrix for class '" << pFilename << "'. Using identity matix.\n";
          TRY((*tmp), identity(tmp->col_count()));
        }
        //
        mICov.push_back(tmp);
        
        if (pFilename != "")
          *mOutput << mName << ": read '" << pFilename << "' (" << mVectorCount << " vectors)\n";
      }
      TRY(mTrainingSet, set_sizes(0,0));
      mVectorCount = 0;
      mMeanValue.clear();
      return true;
    }
    
    mMeanValue += *vector;
    TRY(mTrainingSet, append(*vector));
    
    mVectorCount++;
    return true;
  }
  
  bool load_training_samples(const std::string& pFilename, Matrix * vector)
  {
    if (vector == NULL) {
      // class initialize // finished
      if (!get_label_from_filename(&mLabel, pFilename)) return false;
      return true;
    }
    label_vector_from(*vector, mLabel);
    TRY(mFullTrainingData, append(mLabelVector));
    return true;
  }
  
  void clear_icov()
  {
    for (size_t i = 0; i < mICov.size(); i++)
      delete mICov[i];
    mICov.clear();
  }
  
  virtual void spy()
  { 
    const char * cstr;
    if (mDistanceType == EuclideanDistance)
      cstr = "Euclidean";
    else if (mDistanceType == EuclideanClosestDistance)
      cstr = "Closest";
    else
      cstr = "Mahalanobis";
    bprint(mSpy, mSpySize,"%s %ix%i", cstr, mCodeBook.row_count(), mCodeBook.col_count());  
  }
  
  inline void label_vector_from(const Matrix& pMat, int pLabel)
  {
    if (pLabel < 0)
      // white (no class)
      mLabelVector.data[0] = 0.0;
    else
      mLabelVector.data[0] = mLabel;
    
    for(size_t i=0; i < pMat.size(); i++)
      mLabelVector.data[1+i] = pMat.data[i];
  }
  
  kmeans_distance_types_t mDistanceType; /**< Kind of distance calculation. Default is Euclidean. */
  
  int    mLabel;       /**< Current label. */
  real_t mDistance; /**< Probability for current label. */
  IntMatrix mLabels; /**< List of labels. */
  Matrix mDistances;   /**< Distance to each prototype. */
  real_t mDistanceThreshold;   /**< Minimal distance to send values out. */
  size_t mVectorCount; /**< Number of vectors to compute the current mean value. */
  Matrix mMeanValue; /**< Mean value for the current class. */
  Matrix mCodeBook;  /**< List of prototypes (one row per class). */
  std::vector<Matrix*> mICov; /**< List of inverses of the covariance matrices (one per class) used to compute Mahalanobis distance. */
  Matrix mTrainingSet;   /**< Used during 'learn'. Contains all the training data for one class. */
  Matrix mFullTrainingData; /**< First 3 columns = color from label. */
  Matrix mWork1, mWork2, mWork3; /**< Temporary matrices used to compute Mahalanobis distance. */
  Matrix mLabelVector;   /**< Input vector with color from current label. */
  CutMatrix mView;       /**< Flat view of the incomming matrix. */
};


extern "C" void init()
{
  CLASS (Kmeans)
  OUTLET(Kmeans,label)
  OUTLET(Kmeans,distance)
  OUTLET(Kmeans,label_vector)
  OUTLET(Kmeans,plot)
  METHOD(Kmeans,probabilities)
  METHOD(Kmeans,plot)
  SUPER_METHOD(Kmeans, TrainedMachine, learn)
  SUPER_METHOD(Kmeans, TrainedMachine, load)
}