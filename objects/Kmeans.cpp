#include "trained_machine.h"
#include <float.h> // DBL_MAX

/** Distance calculation algorithms. */
enum kmeans_distance_types_t {
  EuclideanDistance,
  MahalanobisDistance,
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
        send(3, mLabelVector); // input value with current color from label
        send(2, mDistance);
        send(mLabel);
      } else {
        label_vector_from(mView, -1); // out of distance threshold
        send(4, mFullTrainingData);
        send(3, mLabelVector);
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
    double closest_distance = DBL_MAX;
    double total_distance   = 0;
    size_t row_count = mCodeBook.row_count();
    size_t col_count = mCodeBook.col_count();
    mDistances.clear();
    size_t closest_id = -1;
    
    for (size_t i = 0; i < row_count; i++) {
      double d = 0.0;
      if (mDistanceType == EuclideanDistance) {
        for (size_t j = 0; j < col_count; j++)
          d += (mCodeBook.data[i * col_count + j] - live.data[j]) * (mCodeBook.data[i * col_count + j] - live.data[j]);
        d /= col_count;
      } else {
        // Mahalanobis distance d = V * C^{-1} * V'
        mWork1.copy(live);
        mWork1.subtract(mCodeBook, i, i); // remove mean value
        mWork2.mat_multiply(mWork1, *mICov[i]); // w2 = w1 * C^{-1}
        mWork3.mat_multiply(mWork2, mWork1, CblasNoTrans, CblasTrans); // w3 = w2 * w1'
        d = mWork3.data[0] / col_count;
      }
      mDistances.data[i] = d;
      total_distance    += d;
      if (d < closest_distance) {
        closest_id = i;
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
    
    if (mDistances.data[closest_id] < mDistanceThreshold) {
      mLabel = mLabels.data[closest_id];
      mDistance = mDistances.data[closest_id];
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
    
    
    TRY(mLabelVector, set_sizes(1, mCodeBook.col_count() + 3));
    TRY(mFullTrainingData, set_sizes(0, mCodeBook.col_count() + 3));
    
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
        mMeanValue /= (double)mVectorCount;
        
        TRY(mCodeBook, append(mMeanValue));
        
        Matrix * tmp = new Matrix;
        if (mTrainingSet.row_count() > 1) {
          // compute C-1
          // 1. remove mean value
          TRY(mTrainingSet, subtract(mMeanValue));
        
          // 2. compute S'S
          TRY((*tmp), symmetric(mTrainingSet)); // tmp = S'S
          *tmp /= (mTrainingSet.row_count() - 1);
          // 3. find inverse of covariance matrix tmp
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
    bprint(mSpy, mSpySize,"%ix%i", mCodeBook.row_count(), mCodeBook.col_count());  
  }
  
  inline void label_vector_from(const Matrix& pMat, int pLabel)
  {
    if (pLabel < 0) {
      // grey (no class)
      mLabelVector.data[0] = 0.5;
      mLabelVector.data[1] = 0.5;
      mLabelVector.data[2] = 0.5;
    } else {
      uint col_id = hashId((uint)mLabel); // hashId defined in Hash template
      mLabelVector.data[0] = 0.2 + 0.8 * (col_id % 100) / 100.0; // red color
      mLabelVector.data[1] = 0.2 + 0.8 * (col_id % 60)  / 60.0;  // green color
      mLabelVector.data[2] = 0.2 + 0.8 * (col_id % 20)  / 20.0;  // blue color
    }
    
    for(size_t i=0; i < pMat.size(); i++)
      mLabelVector.data[3+i] = pMat.data[i];
  }
  
  kmeans_distance_types_t mDistanceType; /**< Kind of distance calculation. Default is Euclidean. */
  
  int    mLabel;       /**< Current label. */
  double mDistance; /**< Probability for current label. */
  IntMatrix mLabels; /**< List of labels. */
  Matrix mDistances;   /**< Distance to each prototype. */
  double mDistanceThreshold;   /**< Minimal distance to send values out. */
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
  OUTLET(Kmeans,probability)
  OUTLET(Kmeans,label_vector)
  OUTLET(Kmeans,plot)
  METHOD(Kmeans,probabilities)
  METHOD(Kmeans,plot)
  SUPER_METHOD(Kmeans, TrainedMachine, learn)
  SUPER_METHOD(Kmeans, TrainedMachine, load)
}