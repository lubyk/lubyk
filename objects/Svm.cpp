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

class Svm : public TrainedMachine
{
public:
  virtual ~Svm ()
  {
    if (mModel)      svm_destroy_model(mModel);
    if (mNode)       free(mNode);
    if (mProblem.y)  free(mProblem.y);
    if (mProblem.x)  free(mProblem.x);
    if (mXSpace)     free(mXSpace);
    if (mDistances)  free(mXSpace);
    if (mVotes)      free(mVotes);
    if (mLabels)     free(mLabels);
    if (mTrainFile) fclose(mTrainFile);
  }

  bool init(const Params& p)
  {
    if(!init_machine(p)) return false;
    mSampleRate = p.val("rate", 256);
    mUnitSize   = p.val("unit", 1);    // number of values form a sample
    mThreshold  = p.val("threshold", 0.0);
    mSvmCparam  = p.val("cost", 2.0);          // svm cost
    mSvmGammaParam = p.val("gamma", 0.0078125); // svm gamma in RBF
    mProbabilityThreshold = p.val("filter", 0.8);
    
    mLiveBuffer = NULL;
    mTrainFile  = NULL;
    mDistances  = NULL;
    mVotes      = NULL;
    mLabels     = NULL;
    mClassLabel = 0;
    
    mVectorCount = 0; // current mean value made of '0' vectors
    
    // max_nr_attr - 1 > mVectorSize
    mNode = (struct svm_node *) malloc((mVectorSize + 1) * sizeof(struct svm_node));
    if (!mNode) {
      *mOutput << mName << ": Could not allocate " << mVectorSize+1 << " svm_nodes to store vector.\n";
      return false;
    }
    
    mProblem.y = NULL;
    mProblem.x = NULL;
    mXSpace    = NULL;
    
    return do_load_model();
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    if (!mIsOK) return; // do not use 'predict' if no model loaded
    if (sig.type != ArraySignal) return; // ignore
    
    if (sig.array.size >= mVectorSize) {
      mLiveBuffer = sig.array.value;
      mLiveBufferSize = sig.array.size;
    } else {
      *mOutput << mName << ": wrong signal size " << sig.array.size << " should be " << mVectorSize << "\n.";
      return;
    }
    
    //if (mDebug) {
    //  // show current signal value as sparse vector
    //  double * vector = mLiveBuffer + mLiveBufferSize - mVectorSize;
    //  for(int i=0;i < mVectorSize; i++) {
    //    if (vector[i] >= mThreshold || vector[i] <= -mThreshold) {
    //      printf(" %i:%.5f", i+1, (float)vector[i]);
    //    }
    //  }
    //  printf("\n");
    //}
    bool change = predict();
    
    if (mDebug)
      std::cout << mName << ": " << mClassLabel << std::endl;
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
    double labelProbability;
    // map current vector into svm_node
    int i,pos,j = 0; // svm_node index
    double * vector = mLiveBuffer + mLiveBufferSize - mVectorSize;
    
    for(int i=0;i < mVectorSize; i++) {
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
      // use probability estimate
  		svm_predict_values(mModel, mNode, mDistances);

  		for(i=0;i<mLabelCount;i++) mVotes[i] = 0.0;
		
  		pos=0;
      double sum;
  		for(i=0; i < mLabelCount; i++)
  			for(j = i+1; j < mLabelCount; j++)
  			{
          //if (mDebug) printf("%i:%i % .5f\n", i, j, mDistances[pos]);
  				if(mDistances[pos] > 0) {
  					mVotes[i] += mDistances[pos];
  				} else {
  					mVotes[j] -= mDistances[pos];
          }
          pos++;
  			}

  		int vote_max_idx = 0;
  		
      //if (mDebug) printf("================\n");
  		for(i=1; i < mLabelCount; i++) {
        //if (mDebug) printf("%i: %.2f\%\n",i, mVotes[i] * 100.0 / (double)(mLabelCount - 1));
  			if(mVotes[i] > mVotes[vote_max_idx]) vote_max_idx = i;
  		}
  		label = mLabels[vote_max_idx];
      labelProbability = (double)mVotes[vote_max_idx] / (double)(mLabelCount - 1);
		} else {
      label = svm_predict(mModel,mNode);
      labelProbability = 1.0;
    }
    
    if (label == mClassLabel && labelProbability < mLabelProbability) {
      // probability decrease: reached top ==> send value
      mLabelProbability = labelProbability;
      return true;
    }
    
    mClassLabel       = label;
    mLabelProbability = labelProbability;
    return false;
  }
    
  /** Write as a sparse vector. */
  bool write_as_sparse(const std::string& filename, double * vector)
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
    for(int i=0; i < mVectorSize; i++) {
      if (vector[i] >= mThreshold || vector[i] <= -mThreshold) {
        fprintf(mTrainFile, " %i:%.5f", i+1, (float)vector[i]);
      }
    }  
    fprintf(mTrainFile, "\n");
    mVectorCount++;
  }
  
  bool do_load_model ()
  { 
    if (mModel) svm_destroy_model(mModel);
    mModel = svm_load_model(model_file_path().c_str());
    if (!mModel) {
      *mOutput << mName << ": could not load model file '" << model_file_path() << "'.\n";
      return false;
    }
    
    // prepare probability / labels buffer
    mLabelCount = svm_get_nr_class(mModel);
    
    mLabels         = (int*)    malloc(mLabelCount * sizeof(int));
    if (!mLabels) {
      *mOutput << mName << ": could not allocate " << mLabelCount << " ints for labels.\n";
      return false;
    }
    svm_get_labels(mModel, mLabels);
    
		mDistances     = (double*) malloc((mLabelCount * (mLabelCount-1)/2) * sizeof(double));
    if (!mDistances) {
      *mOutput << mName << ": could not allocate " << (mLabelCount * (mLabelCount-1)/2) << " doubles for distances.\n";
      return false;
    }
    
    // vote_count is just a hack because we map probability signals in the same plot as signals packed into
    // mUnitSize elements.
    int vote_count = 0;
    if (mLabelCount < mUnitSize) 
      vote_count = mUnitSize;
    else
      vote_count = mLabelCount;
    mVotes         = (double*) malloc(vote_count * sizeof(double));
    if (!mVotes) {
      *mOutput << mName << ": could not allocate " << vote_count << " ints for votes.\n";
      return false;
    }
    for(int i=0;i<vote_count;i++) mVotes[i] = 0.0;
    
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

    mProblem.y = (double*)            malloc(mProblem.l * sizeof(double));
  	mProblem.x = (struct svm_node **) malloc(mProblem.l * sizeof(struct svm_node *));
  	mXSpace    = (struct svm_node *)  malloc(elements * sizeof(struct svm_node));

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

  FILE * mTrainFile;    /**< Used during foreach class loop to write sparse data. */

  double * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  int mUnitSize;       /**< How many values form a sample (single event). */
  int mVectorCount;    /**< Number of vectors used to build the current mean value. */
  int mSampleRate; /**< How many samples per second do we receive from the 'data' inlet. */
  int mLiveBufferSize;
  int mClassLabel; /**< Current label. Used during recording and recognition. */
  double mLabelProbability; /**< Current probability for the given label. */
  
  double mThreshold;   /**< Values below this limit are recorded as zero in the training set. Use '0' for none. */
  
  /// libsvm ///
  double   mSvmCparam;
  double   mSvmGammaParam;
  double   mProbabilityThreshold; /**< Minimal probability value for an output to be sent. */
  int      mLabelCount;       /**< Number of different classes to recognize. */
  double * mDistances;        /**< Computed distances from each class to other classes. */
  double * mVotes;            /**< Count how many times class X was recognized correctly against other classes. */
  int    * mLabels;           /**< Translate ids to our labels. */
  
  struct svm_node  * mXSpace; /**< x_space (working space used for dot product during training). */
  struct svm_model * mModel;  /**< Contains the model (training result). */
  struct svm_node  * mNode;   /**< Contains our data in sparse format. */
  struct svm_parameter mSvmParam;	/**< Contains settings for the learning phase of svm. */
  struct svm_problem mProblem; /**< ??? */
};


extern "C" void init()
{
  CLASS (Svm)
  OUTLET(Svm,label)
  OUTLET(Svm,probabilities)
  METHOD(Svm,learn)
  METHOD(Svm,build)
}