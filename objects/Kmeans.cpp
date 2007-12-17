#include "trained_machine.h"
#include <float.h> // DBL_MAX

class Kmeans : public TrainedMachine
{
public:
  bool init(const Params& p)
  {
    mCodeBook.set_sizes(0,8); // arbitrary: 0 classes of 8 values
    mDistances.set_sizes(1,8);
    mDistanceThreshold = DBL_MAX;
    return true;
  }
  
  bool set(const Params& p)
  {
    size_t vector_size = mVector.col_count();
    if (p.get(&vector_size, "vector")) {
      TRY(mCodeBook, set_sizes(0,vector_size));
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
      if (get_label(*live)) {
        send(2, mDistance);
        send(mLabel);
      }
    }
  }
  
  void probabilities()
  {
    *mOutput << mDistances << std::endl;
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
      for (size_t j = 0; j < col_count; j++)
        d += (mCodeBook.data[i * col_count + j] - live.data[j]) * (mCodeBook.data[i * col_count + j] - live.data[j]);
      d /= col_count;
      mDistances.data[i] = d;
      total_distance        += d;
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
    TRY(mCodeBook, set_sizes(0,0)); // read all
    
    FILE * file = fopen(model_file_path().c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": could not read model from '" << model_file_path() << "'.\n";
        return false;
      }
      TRY_OR(mLabels,   from_file(file), goto load_model_failed);
      TRY_OR(mCodeBook, from_file(file), goto load_model_failed);
    fclose(file);
    
    TRY(mDistances, set_sizes(1,mCodeBook.row_count()));
    
    return true;
    
    load_model_failed:
    fclose(file);
    return false;
  }
  
  bool learn_from_data()
  {
    TRY(mCodeBook, set_sizes(0, mVector.col_count()));
    TRY(mMeanValue, set_sizes(1, mVector.col_count()));
    TRY(mLabels,   set_sizes(1,0));
    
    if(!FOREACH_TRAIN_CLASS(Kmeans, compute_mean_vector)) {
      *mOutput << mName << ": could not build model.\n";
      return false;
    }
    
    *mOutput << mName << ": built codebook of size " << mCodeBook.row_count() << "x" << mCodeBook.col_count() << ".\n";
    TRY(mLabels,   to_file(model_file_path()));
    TRY(mCodeBook, to_file(model_file_path(), "ab"));
    return true;
  }
  
  bool compute_mean_vector(const std::string& pFilename, Matrix * vector)
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
        
        if (pFilename != "")
          *mOutput << mName << ": read '" << pFilename << "' (" << mVectorCount << " vectors)\n";
      }
      
      mVectorCount = 0;
      mMeanValue.clear();
      return true;
    }
    
    mMeanValue += *vector;
    
    mVectorCount++;
    return true;
  }
  
  virtual void spy()
  { 
    bprint(mSpy, mSpySize,"%ix%i", mCodeBook.row_count(), mCodeBook.col_count());  
  }
  
  int    mLabel;       /**< Current label. */
  double mDistance; /**< Probability for current label. */
  IntMatrix mLabels; /**< List of labels. */
  Matrix mDistances;   /**< Distance to each prototype. */
  double mDistanceThreshold;   /**< Minimal distance to send values out. */
  size_t mVectorCount; /**< Number of vectors to compute the current mean value. */
  Matrix mMeanValue; /**< Mean value for the current class. */
  Matrix mCodeBook;  /**< List of prototypes (one row per class). */
};


extern "C" void init()
{
  CLASS (Kmeans)
  OUTLET(Kmeans,label)
  OUTLET(Kmeans,probability)
  METHOD(Kmeans,probabilities)
  SUPER_METHOD(Kmeans, TrainedMachine, learn)
  SUPER_METHOD(Kmeans, TrainedMachine, load)
}