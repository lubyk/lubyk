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


#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

// declarations not in svm.h
Real sigmoid_predict(Real decision_value, Real A, Real B);
#ifndef min
template <class T> inline T min(T x,T y) { return (x<y)?x:y; }
#endif
#ifndef max
template <class T> inline T max(T x,T y) { return (x>y)?x:y; }
#endif


class Svm : public TrainedMachine
{
public:
  Svm() : mModel(NULL), mNode(NULL), mXSpace(NULL), mPairwiseProb(NULL), mVotes(NULL), mLabels(NULL), mTrainFile(NULL), mLiveBuffer(NULL), class_Label(0), mVectorCount(0)
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

  bool init(const Value &p)
  {
    mProblem.y = NULL;
    mProblem.x = NULL;
    mXSpace    = NULL;
    mThreshold  = 0.0;
    mSvmCparam  = 2.0; // svm cost
    mSvmGammaParam = 0.0078125; // svm gamma in RBF
    mProbabilityThreshold = 0.8;
    return init_machine(p);
  }
  
  bool set (const Value &p)
  {
    size_t vector_size = mVector.col_count();
    if(!set_machine(p)) return false;
    p.get(&vector_size, "vector");
    p.get(&mThreshold, "threshold");
    p.get(&mSvmCparam, "cost"); // svm cost
    p.get(&mSvmGammaParam, "gamma"); // svm gamma in RBF
    p.get(&mProbabilityThreshold, "filter");
    
    // max_nr_attr - 1 > mVectorSize
    if (!mNode)
      mNode = (struct svm_node *) malloc((vector_size + 1) * sizeof(struct svm_node));
    else if (vector_size != mVector.col_count()) {
      void * tmp = realloc(mNode, (vector_size + 1) * sizeof(struct svm_node));
      if (!tmp) {
        *output_ << name_ << ": could not reallocate " << vector_size + 1 << " nodes.\n";
        return false;
      }
      mNode = (struct svm_node *) tmp;
    }
    
    return set_machine(p);
  }

  // inlet 1
  void bang (const Value &val)
  {
    if (!is_ok_) return; // do not use 'predict' if no model loaded
    if (val.type != MatrixValue) return; // ignore
    
    if (val.matrix.value->col_count() >= mVector.col_count()) {
      mLiveBuffer = val.matrix.value;
    } else {
      *output_ << name_ << ": wrong signal size " << val.matrix.value->col_count() << " should be " << mVector.col_count() << "\n.";
      return;
    }
    
    //bool change = 
    predict();
    
    //if (change) {
    send(2, mLabelProbability);
    send(class_Label);
    //}
  }
  
  void build()
  {
    if (!do_build()) {
      *output_ << name_ << ": could not build sparse data file.\n";
    }
  }
  
  
  virtual const Value inspect(const Value &val) 
  {  
    bprint(mSpy, mSpySize,"C:%.1f g:%.6f", mSvmCparam, mSvmGammaParam);    
  }
  
private:
  
  
  // Method 2 from the multiclass_prob paper by Wu, Lin, and Weng
  void multiclass_probability(int k, Real *r, Real *p)
  {
  	int t,j;
  	int iter = 0, max_iter=max(100,k);
  	Real **Q=Malloc(Real *,k);
  	Real *Qp=Malloc(real_t,k);
  	Real pQp, eps=0.005/k;

  	for (t=0;t<k;t++)
  	{
  		p[t]=1.0/k;  // Valid if k = 1
  		Q[t]=Malloc(real_t,k);
  		Q[t][t]=0;
  		for (j=0;j<t;j++)
  		{
  			Q[t][t]+=r[j * mLabelCount + t]*r[j * mLabelCount + t];
  			Q[t][j]=Q[j][t];
  		}
  		for (j=t+1;j<k;j++)
  		{
  			Q[t][t]+=r[j* mLabelCount + t]*r[j * mLabelCount + t];
  			Q[t][j]=-r[j* mLabelCount + t]*r[t * mLabelCount + j];
  		}
  	}
  	for (iter=0;iter<max_iter;iter++)
  	{
  		// stopping condition, recalculate QP,pQP for numerical accuracy
  		pQp=0;
  		for (t=0;t<k;t++)
  		{
  			Qp[t]=0;
  			for (j=0;j<k;j++)
  				Qp[t]+=Q[t][j]*p[j];
  			pQp+=p[t]*Qp[t];
  		}
  		Real max_error=0;
  		for (t=0;t<k;t++)
  		{
  			Real error=fabs(Qp[t]-pQp);
  			if (error>max_error)
  				max_error=error;
  		}
  		if (max_error<eps) break;

  		for (t=0;t<k;t++)
  		{
  			Real diff=(-Qp[t]+pQp)/Q[t][t];
  			p[t]+=diff;
  			pQp=(pQp+diff*(diff*Q[t][t]+2*Qp[t]))/(1+diff)/(1+diff);
  			for (j=0;j<k;j++)
  			{
  				Qp[j]=(Qp[j]+diff*Q[t][j])/(1+diff);
  				p[j]/=(1+diff);
  			}
  		}
  	}
  	if (iter>=max_iter)
  		error("Exceeds max_iter in multiclass_prob\n");
  	for(t=0;t<k;t++) free(Q[t]);
  	free(Q);
  	free(Qp);
  }
  
  /** Transform the raw data into the sparse format used by libsvm. 
    * Use mThreshold to fix what is considered as zero. This outputs the file 'svm.train' containing 
    * all data with labels from the classes in mFolder. */
  bool do_build()
  {
    if (mTrainFile) fclose(mTrainFile);
    mTrainFile = fopen(train_file_path().c_str(), "wb");
      if (!mTrainFile) {
        *output_ << name_ << ": could not open '" << train_file_path() << "' to store training data.\n";
        return false;
      }
    
      mVectorCount = 0;
      if(!FOREACH_TRAIN_CLASS(Svm, write_as_sparse)) {
        *output_ << name_ << ": could not write as sparse data.\n";
        return false;
      }
    
    fclose(mTrainFile);
    return true;
  }
  
  /** Update the model based on the current recorded samples. */
  bool learn_from_data()
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
      *output_ << name_ << ": could not use training data '" << trainFilePath << "'.\n";
      return false;
  	}
  	
  	// check param/data is ok
  	
  	errorMsg = svm_check_parameter(&mProblem,&mSvmParam);

  	if(errorMsg) {
      *output_ << name_ << ": " << errorMsg << "\n.";
      return false;
  	}
    // 2. train with these params
    mModel = svm_train(&mProblem, &mSvmParam);
    svm_save_model(model_file_path().c_str(),mModel);
    
    return true;
  }
  
  
  bool predict()
  {
    int label;
    size_t vector_size = mVector.col_count();
    Real labelProbability = 1.0;
    
    // map current vector into svm_node
    size_t j = 0; // svm_node index
    Real * vector = mLiveBuffer->data + mLiveBuffer->size() - vector_size;
    
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
      class_Label = svm_predict(mModel,mNode);
      return true;
    }
    
    if (true || labelProbability > mProbabilityThreshold) {
      // confident enough, use this label
      class_Label = label;
      mLabelProbability = labelProbability;
      return true;
    }
    return false;
  }
    
  /** Write as a sparse vector. */
  bool write_as_sparse(const std::string &pFilename, Matrix * pVector)
  {
    if (!pVector) {
      // class initialize // finish
      if (!get_label_from_filename(&class_Label, pFilename)) return false;
      return true;
    }
    if (!mTrainFile) {
      *output_ << name_ << ": train file not opened !.\n";
      return false;
    }
    fprintf(mTrainFile, "%+i", class_Label);
    for(size_t i=0; i < pVector->col_count(); i++) {
      if (pVector->data[i] >= mThreshold || pVector->data[i] <= -mThreshold) {
        fprintf(mTrainFile, " %i:%.5f", (int)i+1, (float)pVector->data[i]);
      }
    }  
    fprintf(mTrainFile, "\n");
    mVectorCount++;
    return true;
  }
  
  bool load_model ()
  { 
    if (mModel) svm_destroy_model(mModel);
    mModel = svm_load_model(model_file_path().c_str());
    if (!mModel) {
      *output_ << name_ << ": could not load model file '" << model_file_path() << "'.\n";
      return false;
    }
    
    // prepare probability / label buffers
    mLabelCount = svm_get_nr_class(mModel);
    
    if (!allocate<size_t>(&mVotes,  mLabelCount, "votes", "sizes"))  return false;
    if (!alloc_ints(&mLabels, mLabelCount, "labels")) return false;
    
    svm_get_labels(mModel, mLabels);
    *output_ << name_ << ": " << mLabelCount << " labels:\n";
    for(size_t i=0; i < mLabelCount; i++) {
      *output_ << " " << mLabels[i];
    }
    *output_ << "\n";
    
    if (!alloc_reals(&mDistances,    mLabelCount * (mLabelCount-1)/2, "pairwise distances")) return false;
    if (!alloc_reals(&mPairwiseProb, mLabelCount * mLabelCount,   "pairwise probabilities")) return false;
    
    return true;
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
      *output_ << name_ << ": could not open training data '" << filename << "'\n.";
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

    
    if (!alloc_reals(&mProblem.y, mProblem.l, "problem.y")) goto readpb_fail;
    if (!allocate<struct svm_node *>(&mProblem.x, mProblem.l, "problem.x", "svm_node pointers")) goto readpb_fail;
    if (!allocate<struct svm_node>(  &mXSpace, elements, "mXSpace", "svm_nodes")) goto readpb_fail;
    //mProblem.y = (real_t*)            malloc(mProblem.l * sizeof(real_t));
  	//mProblem.x = (struct svm_node **) malloc(mProblem.l * sizeof(struct svm_node *));
  	//mXSpace    = (struct svm_node *)  malloc(elements * sizeof(struct svm_node));

  	max_index = 0;
  	j=0;
  	for(i=0;i<mProblem.l;i++)
  	{
  		Real label;
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
          *output_ << name_ << ": wrong input format at line " << i+1 << "\n";
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
  				*output_ << name_ << ": (error): wrong input format: first column must be 0:sample_serial_number\n";
          goto readpb_fail;
  			}
  			if ((int)mProblem.x[i][0].value <= 0 || (int)mProblem.x[i][0].value > max_index)
  			{
  				*output_ << name_ << ": (error): wrong input format: sample_serial_number out of range\n";
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
  void get_label(int * pLabel, Real * pProbability)
  {
  	if (svm_get_svm_type(mModel) == C_SVC || svm_get_svm_type(mModel) == NU_SVC)
  	{
      Real uniform_probability = 1.0 / mLabelCount;
  		svm_predict_values(mModel, mNode, mDistances);
  		
  		int k=0;

  		Real min_prob=1e-7;
      Real prob;
  		
      k=0;
  		memset(mVotes, 0, mLabelCount * sizeof(size_t));
      
  		for(size_t i=0;i<mLabelCount;i++)
  			for(size_t j=i+1;j<mLabelCount;j++)
  			{
  			  // the min(max( sigmoid )) thing is to force the range to be in [min_prob..1-min_prob]
  			  prob = min( max( sigmoid_predict( mDistances[k], uniform_probability, uniform_probability),
  				                         min_prob), 1-min_prob);
  				
  				// i = 0
  				// j = 1
          mPairwiseProb[i*mLabelCount + j] = prob;       // 0 + 1
  				mPairwiseProb[j*mLabelCount + i] = 1 - prob;   // 2 + 0
  				
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
      Real bad_prob = 1.0; // we start by beeing optimistic
      Real * probs = mPairwiseProb + vote_id * mLabelCount;
      for(size_t i = 0; i < mLabelCount; i++) {
        if (i == vote_id) continue;
        if (probs[i] < bad_prob) bad_prob = probs[i];
      }

      *pLabel = mLabels[vote_id];
      *pProbability = bad_prob;
      
  		// compute 'prob_estimates' (see http://citeseer.ist.psu.edu/wu03probability.html)
  		
      // Matrix prob_vector;
      // TRY_RET(prob_vector, set_sizes(1, mLabelCount));
      // prob_vector.fill(0.0);
  		// multiclass_probability(mLabelCount,mPairwiseProb,prob_vector.data);
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
  Real   mSvmCparam;
  Real   mSvmGammaParam;
  Real   mProbabilityThreshold; /**< Minimal probability value for an output to be sent. */
  size_t   mLabelCount;       /**< Number of different classes to recognize. */
  
  struct svm_model * mModel;  /**< Contains the model (training result). */
  struct svm_node  * mNode;   /**< Contains our data in sparse format. */
  struct svm_node  * mXSpace; /**< x_space (working space used for dot product during training). */
  struct svm_parameter mSvmParam;	/**< Contains settings for the learning phase of svm. */
  struct svm_problem mProblem; /**< Packaging of the problem definition, training data and parameters. */
  
  Real * mPairwiseProb;     /**< Pairwise probabilities between classes. */
  Real * mDistances;        /**< Pairwise distances to the hyper-plane. */
  size_t * mVotes;            /**< Used to compute the label out of the distances. */
  int    * mLabels;           /**< Translation from 'ids' to 'labels'. */
  
  FILE * mTrainFile;          /**< Used during foreach class loop to write sparse data. */

  const Matrix * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  int    class_Label;         /**< Current label. Used during recording and recognition. */
  size_t mVectorCount;        /**< Number of vectors used to build the current mean value. */
  
  Real mLabelProbability;   /**< Current probability for the given label. */
  
  Real mThreshold;          /**< Values below this limit are recorded as zero in the training set. Use '0' for none. */
};


extern "C" void init(Planet &planet) {
  CLASS (Svm)
  OUTLET(Svm,label)
  OUTLET(Svm,probability)
  SUPER_METHOD(Svm, TrainedMachine, learn)
  SUPER_METHOD(Svm, TrainedMachine, load)
  METHOD(Svm,build)
}