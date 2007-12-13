/** ************ COPYRIGHT NOTICE FOR read_problem method ******************* */
/** This file uses code adapted from svm-predict.c :

Copyright (c) 2000-2007 Chih-Chung Chang and Chih-Jen Lin
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither name of copyright holders nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********* */

#include "trained_machine.h"

// libsvm
#include "svm/svm.h"



// declarations not in svm.h
double sigmoid_predict(double decision_value, double A, double B);
#ifndef min
template <class T> inline T min(T x,T y) { return (x<y)?x:y; }
#endif
#ifndef max
template <class T> inline T max(T x,T y) { return (x>y)?x:y; }
#endif


class Svm : public TrainedMachine
{
public:
  Svm() : mModel(NULL), mNode(NULL), mXSpace(NULL), mPairwiseProb(NULL), mVotes(NULL), mLabels(NULL), mTrainFile(NULL), mLiveBuffer(NULL), mClassLabel(0), mVectorCount(0)
  {
    mProblem.x = NULL; 
    mProblem.y = NULL;
  }
  virtual ~Svm ()
  {
    if (mModel)      svm_destroy_model(mModel);
    if (mNode)       free(mNode);
    if (mProblem.y)  free(mProblem.y);
    if (mProblem.x)  free(mProblem.x);
    if (mXSpace)     free(mXSpace);
    if (mDistances)  free(mDistances);
    if (mPairwiseProb)  free(mPairwiseProb);
    if (mVotes)      free(mVotes);
    if (mLabels)     free(mLabels);
    if (mTrainFile)  fclose(mTrainFile);
  }

  bool init(const Params& p)
  {
    mProblem.y = NULL;
    mProblem.x = NULL;
    mXSpace    = NULL;
    mThreshold  = 0.0;
    mSvmCparam  = 2.0; // svm cost
    mSvmGammaParam = 0.0078125; // svm gamma in RBF
    mProbabilityThreshold = 0.8;
    return set(p);
  }
  
  bool set (const Params& p)
  {
    size_t vector_size = mVector.col_count();
    if(!set_machine(p)) return false;
    p.get(&mThreshold, "threshold");
    p.get(&mSvmCparam, "cost"); // svm cost
    p.get(&mSvmGammaParam, "gamma"); // svm gamma in RBF
    p.get(&mProbabilityThreshold, "filter");
    
    // max_nr_attr - 1 > mVectorSize
    if (!mNode)
      mNode = (struct svm_node *) malloc((mVector.col_count() + 1) * sizeof(struct svm_node));
    else if (vector_size != mVector.col_count()) {
      void * tmp = realloc(mNode, (mVector.col_count() + 1) * sizeof(struct svm_node));
      if (!tmp) {
        *mOutput << mName << ": could not reallocate " << mVector.col_count() + 1 << " nodes.\n";
        return false;
      }
      mNode = (struct svm_node *) tmp;
    }
    
    return do_load_model();
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    if (!mIsOK) return; // do not use 'predict' if no model loaded
    if (sig.type != MatrixSignal) return; // ignore
    
    if (sig.matrix.value->col_count() >= mVector.col_count()) {
      mLiveBuffer = sig.matrix.value;
    } else {
      *mOutput << mName << ": wrong signal size " << sig.matrix.value->col_count() << " should be " << mVector.col_count() << "\n.";
      return;
    }
    
    bool change = predict();
    
    if (change) send(mClassLabel);    
  }
  
  
  /** Accessor from command line. */
  void learn()
  {
    if (!do_learn()) {
      *mOutput << mName << ": learn failed.\n";
      mIsOK = false;
    } else {
      mIsOK = true;
    }
  }
  
  void build()
  {
    if (!do_build()) {
      *mOutput << mName << ": could not build sparse data file.\n";
    }
  }
  
  void load()
  {
    if (!do_load_model()) {
      *mOutput << mName << ": could not load model.\n";
      mIsOK = false;
    } else {
      mIsOK = true;
    }
  }
private:
  
  
  /** Transform the raw data into the sparse format used by libsvm. 
    * Use mThreshold to fix what is considered as zero. This outputs the file 'svm.train' containing 
    * all data with labels from the classes in mFolder. */
  bool do_build()
  {
    if (mTrainFile) fclose(mTrainFile);
    mTrainFile = fopen(train_file_path().c_str(), "wb");
      if (!mTrainFile) {
        *mOutput << mName << ": could not open '" << train_file_path() << "' to store training data.\n";
        return false;
      }
    
      mVectorCount = 0;
      if(!FOREACH_TRAIN_CLASS(Svm, write_as_sparse)) {
        *mOutput << mName << ": could not write as sparse data.\n";
        return false;
      }
    
    fclose(mTrainFile);
    return true;
  }
  
  /** Update the model based on the current recorded samples. */
  bool do_learn()
  {
    if (!do_build()) return false;
    const char * errorMsg;
    // 1. find best params (see grid.py) (or use default...) C=2.0 gamma=0.0078125
    // use default values
  	mSvmParam.svm_type = C_SVC;
  	mSvmParam.kernel_type = RBF;
  	mSvmParam.degree = 3;
  	mSvmParam.gamma = mSvmGammaParam;	// 1/k
  	mSvmParam.coef0 = 0;
  	mSvmParam.nu = 0.5;
  	mSvmParam.cache_size = 100;
  	mSvmParam.C = mSvmCparam;
  	mSvmParam.eps = 1e-3;
  	mSvmParam.p = 0.1;
  	mSvmParam.shrinking = 1;
  	mSvmParam.probability = 0;
  	mSvmParam.nr_weight = 0;
  	mSvmParam.weight_label = NULL;
  	mSvmParam.weight = NULL;
  	
  	// read training dataset
  	std::string trainFilePath(mFolder);
    trainFilePath.append("/svm.train");
    
  	if (!read_problem(trainFilePath.c_str())) {
      *mOutput << mName << ": could not use training data '" << trainFilePath << "'.\n";
      return false;
  	}
  	
  	// check param/data is ok
  	
  	errorMsg = svm_check_parameter(&mProblem,&mSvmParam);

  	if(errorMsg) {
      *mOutput << mName << ": " << errorMsg << "\n.";
      return false;
  	}
    // 2. train with these params
    mModel = svm_train(&mProblem, &mSvmParam);
    svm_save_model(model_file_path().c_str(),mModel);
    
    return do_load_model();
  }
  
  
  bool predict()
  {
    int label;
    size_t vector_size = mVector.col_count();
    double labelProbability = 1.0;
    
    // map current vector into svm_node
    size_t j = 0; // svm_node index
    double * vector = mLiveBuffer->data + mLiveBuffer->size() - vector_size;
    
    for(size_t i=0;i < vector_size; i++) {
      if (vector[i] >= mThreshold || vector[i] <= -mThreshold) {
        //printf(" %i:%.5f", i+1, (float)vector[i]);
        mNode[j].index = i+1;
        mNode[j].value = vector[i];
        j++;
      }
    }
    //printf("\n");
    mNode[j].index = -1; // end of vector definition
    
    if (mLabelCount > 1) {
      // use probability threshold
      get_label(&label, &labelProbability);
		} else {
      label = svm_predict(mModel,mNode);
    }
    
    if (labelProbability > mProbabilityThreshold) {
      // confident enough, use this label
      mClassLabel = label;
      mLabelProbability = labelProbability;
      return true;
    }
    return false;
  }
    
  /** Write as a sparse vector. */
  bool write_as_sparse(const std::string& filename, Matrix * vector)
  {
    if (!vector) {
      // class initialize // finish
      size_t start = filename.find("class_");
      size_t end   = filename.find(".txt");
      if (start == std::string::npos || end == std::string::npos) {
        *mOutput << mName << ": bad filename '" << filename << "'. Should be 'class_[...].txt'.\n";
        return false;
      }
      mClassLabel = atoi(filename.substr(start+6, end - start - 6).c_str());
      return true;
    }
    if (!mTrainFile) {
      *mOutput << mName << ": train file not opened !.\n";
      return false;
    }
    fprintf(mTrainFile, "%+i", mClassLabel);
    for(size_t i=0; i < vector->col_count(); i++) {
      if (vector->data[i] >= mThreshold || vector->data[i] <= -mThreshold) {
        fprintf(mTrainFile, " %i:%.5f", (int)i+1, (float)vector->data[i]);
      }
    }  
    fprintf(mTrainFile, "\n");
    mVectorCount++;
    return true;
  }
  
  bool do_load_model ()
  { 
    if (mModel) svm_destroy_model(mModel);
    mModel = svm_load_model(model_file_path().c_str());
    if (!mModel) {
      *mOutput << mName << ": could not load model file '" << model_file_path() << "'.\n";
      return false;
    }
    
    // prepare probability / label buffers
    mLabelCount = svm_get_nr_class(mModel);
    
    if (!allocate<size_t>(&mVotes,  mLabelCount, "votes", "sizes"))  return false;
    if (!alloc_ints(&mLabels, mLabelCount, "labels")) return false;
    
    svm_get_labels(mModel, mLabels);
    
    if (!alloc_doubles(&mDistances,    mLabelCount * (mLabelCount-1)/2, "pairwise distances")) return false;
    if (!alloc_doubles(&mPairwiseProb, mLabelCount * mLabelCount,   "pairwise probabilities")) return false;
    
    
    return true;
  }
  
  std::string model_file_path()
  {
    std::string modelFilePath(mFolder);
    modelFilePath.append("/svm.model");
    return modelFilePath;
  }
  
  std::string train_file_path()
  {
    std::string trainFilePath(mFolder);
    trainFilePath.append("/svm.train");
    return trainFilePath;
  }
  
  
  ////////  adapted from svm-predict.c  ////////

  // read in a problem (in svmlight format)

  bool read_problem(const char *filename)
  {
  	int elements, max_index, i, j;
  	FILE *fp = fopen(filename,"r");

  	if(!fp) {
      *mOutput << mName << ": could not open training data '" << filename << "'\n.";
      return false;
  	}

  	mProblem.l = 0;
  	elements = 0;
  	while(1)
  	{
  		int c = fgetc(fp);
  		switch(c)
  		{
  			case '\n':
  				++mProblem.l;
  				// fall through,
  				// count the '-1' element
  			case ':':
  				++elements;
  				break;
  			case EOF:
  				goto out;
  			default:
  				;
  		}
  	}
  out:
  	rewind(fp);

    
    if (!alloc_doubles(&mProblem.y, mProblem.l, "problem.y")) goto readpb_fail;
    if (!allocate<struct svm_node *>(&mProblem.x, mProblem.l, "problem.x", "svm_node pointers")) goto readpb_fail;
    if (!allocate<struct svm_node>(  &mXSpace, elements, "mXSpace", "svm_nodes")) goto readpb_fail;
    //mProblem.y = (double*)            malloc(mProblem.l * sizeof(double));
  	//mProblem.x = (struct svm_node **) malloc(mProblem.l * sizeof(struct svm_node *));
  	//mXSpace    = (struct svm_node *)  malloc(elements * sizeof(struct svm_node));

  	max_index = 0;
  	j=0;
  	for(i=0;i<mProblem.l;i++)
  	{
  		double label;
  		mProblem.x[i] = &mXSpace[j];
  		fscanf(fp,"%lf",&label);
  		mProblem.y[i] = label;

  		while(1)
  		{
  			int c;
  			do {
  				c = getc(fp);
  				if(c=='\n') goto readpb_out2;
  			} while(isspace(c));
  			ungetc(c,fp);
  			if (fscanf(fp,"%d:%lf",&(mXSpace[j].index),&(mXSpace[j].value)) < 2)
  			{
          *mOutput << mName << ": wrong input format at line " << i+1 << "\n";
          goto readpb_fail;
  			}
  			++j;
  		}	
readpb_out2:
  		if(j>=1 && mXSpace[j-1].index > max_index)
  			max_index = mXSpace[j-1].index;
  		mXSpace[j++].index = -1;
  	}

  	if(mSvmParam.gamma == 0)
  		mSvmParam.gamma = 1.0/max_index;

  	if(mSvmParam.kernel_type == PRECOMPUTED)
  		for(i=0;i<mProblem.l;i++)
  		{
  			if (mProblem.x[i][0].index != 0)
  			{
  				*mOutput << mName << ": (error): wrong input format: first column must be 0:sample_serial_number\n";
          goto readpb_fail;
  			}
  			if ((int)mProblem.x[i][0].value <= 0 || (int)mProblem.x[i][0].value > max_index)
  			{
  				*mOutput << mName << ": (error): wrong input format: sample_serial_number out of range\n";
          goto readpb_fail;
  			}
  		}

    fclose(fp);
    return true;
readpb_fail:
    fclose(fp);
    return false;
  }

  ////////  adapted from svm.cpp, svm_predict_probability  ////////

  /** Finds the best label in a multiclass problem. Returns some kind of confidence value with
    * the worst pairwize probability for this label against the other classes. */
  void get_label(int * pLabel, double * pProbability)
  {
  	if (svm_get_svm_type(mModel) == C_SVC || svm_get_svm_type(mModel) == NU_SVC)
  	{
      double uniform_probability = 1.0 / mLabelCount;
  		svm_predict_values(mModel, mXSpace, mDistances);

  		double min_prob=1e-7;
      double prob;
  		
  		int k=0;
  		memset(mVotes, 0, mLabelCount);
      
  		for(size_t i=0;i<mLabelCount;i++)
  			for(size_t j=i+1;j<mLabelCount;j++)
  			{
  			  // the min(max( sigmoid )) thing is to force the range to be in [min_prob..1-min_prob]
  			  prob = min( max( sigmoid_predict( mDistances[k], uniform_probability, uniform_probability),
  				                         min_prob), 1-min_prob);
  				
          mPairwiseProb[i*mLabelCount + j] = prob;
  				mPairwiseProb[j*mLabelCount + i] = 1 - prob;
  				
  				if (mDistances[k] > 0)
            mVotes[i]++;
          else
            mVotes[j]++;
            
  				k++;
  			}
  		
      size_t vote_id = 0;
      for(size_t i=1; i < mLabelCount; i++)
        if (mVotes[i] > mVotes[vote_id]) vote_id = i;
      
      // our label id is 'vote_id'
      // Let's find it's worst pairwise probability:
      double bad_prob = 1.0; // we start by beeing optimistic
      double * probs = mPairwiseProb + vote_id * mLabelCount;
      size_t row_index = vote_id * mLabelCount;
      for(size_t i = 0; i < mLabelCount; i++) {
        if (i == vote_id) continue;
        if (probs[row_index + i] < bad_prob) bad_prob = probs[row_index + i];
      }

      *pLabel = mLabels[vote_id];
      *pProbability = bad_prob;
      
  		// compute 'prob_estimates' (see http://citeseer.ist.psu.edu/wu03probability.html)
  		//multiclass_probability(mLabelCount,pairwise_prob,prob_estimates);
      //
      //int prob_max_idx = 0;
      //for(i=1;i<mLabelCount;i++)
      //  if(prob_estimates[i] > prob_estimates[prob_max_idx])
      //    prob_max_idx = i;
      //return mModel->label[prob_max_idx];
      
  	} else {
  	  // cannot use probabilities
      *pLabel = svm_predict(mModel, mXSpace);
      // *pProbability not set
  	}
  }
  
  /// libsvm ///
  double   mSvmCparam;
  double   mSvmGammaParam;
  double   mProbabilityThreshold; /**< Minimal probability value for an output to be sent. */
  size_t   mLabelCount;       /**< Number of different classes to recognize. */
  
  struct svm_model * mModel;  /**< Contains the model (training result). */
  struct svm_node  * mNode;   /**< Contains our data in sparse format. */
  struct svm_node  * mXSpace; /**< x_space (working space used for dot product during training). */
  struct svm_parameter mSvmParam;	/**< Contains settings for the learning phase of svm. */
  struct svm_problem mProblem; /**< Packaging of the problem definition, training data and parameters. */
  
  double * mPairwiseProb;     /**< Pairwise probabilities between classes. */
  double * mDistances;        /**< Pairwise distances to the hyper-plane. */
  size_t * mVotes;            /**< Used to compute the label out of the distances. */
  int    * mLabels;           /**< Translation from 'ids' to 'labels'. */
  
  FILE * mTrainFile;          /**< Used during foreach class loop to write sparse data. */

  const Matrix * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  int    mClassLabel;         /**< Current label. Used during recording and recognition. */
  size_t mVectorCount;        /**< Number of vectors used to build the current mean value. */
  
  double mLabelProbability;   /**< Current probability for the given label. */
  
  double mThreshold;          /**< Values below this limit are recorded as zero in the training set. Use '0' for none. */
};


extern "C" void init()
{
  CLASS (Svm)
  OUTLET(Svm,label)
  OUTLET(Svm,probabilities)
  METHOD(Svm,learn)
  METHOD(Svm,build)
}