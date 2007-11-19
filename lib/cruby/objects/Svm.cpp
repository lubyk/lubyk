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

#include "class.h"
#include <errno.h>   /* Error number definitions */
#include <sys/types.h> // directory listing
#include <dirent.h>    // directory listing

// libsvm
#include "svm/svm.h"

/** Svm states. */
enum svm_states_t {
  ReadyToRecord,     /**< Ready to record new data. */
  CountDownReady,    /**< Send events to say "ready" */
  CountDownSet,      /**< Send events to say "set"   */
  Recording,         /**< Send events to say "go!" */
  Validation,        /**< Waits for user confirmation (any char = save & record next, del = do not save), esc = exit. */
  Learning,          /**< Thread launched to learn the new data. Only possible action is interrupt. */
  Label,             /**< Done learning or restored from file. Outputs labels. */
};

class Svm : public Node
{
public:
  virtual ~Svm ()
  {
    if (mBuffer)     free(mBuffer);
    if (mMeanVector) free(mMeanVector);
    if (mModel)      svm_destroy_model(mModel);
    if (mNode)       free(mNode);
    if (mProblem.y)  free(mProblem.y);
    if (mProblem.x)  free(mProblem.x);
    if (mXSpace)     free(mXSpace);
    if (mDistances)  free(mXSpace);
    if (mVotes)      free(mVotes);
    if (mLabels)     free(mLabels);
  }

  bool init(const Params& p)
  {
    mVectorSize = p.val("vector", 32);
    mSampleRate = p.val("rate", 256);
    mUnitSize   = p.val("unit", 1);    // number of values form a sample
    mMargin     = p.val("margin", 1.0);
    mFolder     = p.val("store", std::string("svm_data"));
    mThreshold  = p.val("threshold", 0.0);
    mSvmCparam  = p.val("C", 2.0);          // svm cost
    mSvmGammaParam = p.val("g", 0.0078125); // svm gamma in RBF
    mProbabilityThreshold = p.val("filter", 0.8);
    
    mBufferSize = mVectorSize * (1.0 + mMargin);
    mLiveBuffer = NULL;
    mTrainFile  = NULL;
    mDistances  = NULL;
    mVotes      = NULL;
    mLabels     = NULL;
    mClassLabel = 0;
    
    mVectorCount = 0; // current mean value made of '0' vectors
    mMeanVector = (double*)malloc(mVectorSize * sizeof(double));
    if (!mMeanVector) {
      *mOutput << mName << ": Could not allocate " << mVectorSize << " doubles for mean vector.\n";
      return false;
    } else {
      // clear mMeanVector
      for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    }
    
    mBuffer     = (double*)malloc(mBufferSize * sizeof(double));
    if (!mBuffer) {
      *mOutput << mName << ": Could not allocate " << mBufferSize << " doubles for buffer.\n";
      return false;
    }
    
    // max_nr_attr - 1 > mVectorSize
    mNode = (struct svm_node *) malloc((mVectorSize + 1) * sizeof(struct svm_node));
    if (!mNode) {
      *mOutput << mName << ": Could not allocate " << mVectorSize+1 << " svm_nodes to store vector.\n";
      return false;
    }
    
    mProblem.y = NULL;
    mProblem.x = NULL;
    mXSpace    = NULL;
    
    
    load_model();
    return true;
  }

  // inlet 1
  void bang (const Signal& sig)
  {
    int cmd;
    
    time_t record_time = (time_t)(ONE_SECOND * mVectorSize / (mSampleRate * mUnitSize));
    time_t record_with_margin = (time_t)(ONE_SECOND * mVectorSize * (1 + mMargin/2.0) / (mSampleRate * mUnitSize));
    time_t countdown_time;
    if (record_time > 500)
      countdown_time = record_time;
    else
      countdown_time = 500;
    
    if (!mIsOK) return; // no recovery
    if (sig.type == ArraySignal && sig.array.size >= mBufferSize) {
      mLiveBuffer = sig.array.value;
      mLiveBufferSize = sig.array.size;
    } else {
      // receiving Bangs or command change
      sig.get(&cmd);
      
      switch(mState) {
      case CountDownReady:  
        bang_me_in(countdown_time);
        send_note(60 + (mClassLabel % 12),80,100,1,0,2);
        enter(CountDownSet);
        break;
      case CountDownSet:
        bang_me_in(record_with_margin); // 1/2 margin at the end
        send_note(72 + (mClassLabel % 12),80,record_time,1,0,2);
        enter(Recording);
        break;
      case Recording:
        receive_data();
        enter(Validation);
        break;
      case Validation:
        if (cmd == 127) {
          // backspace ignore current vector
          enter(ReadyToRecord);
          break;
        } else if (cmd == ' ') {
          // swap snap style
          if (mUseVectorOffset == mVectorOffset) {
            mUseVectorOffset = mBufferSize - mVectorSize;
            *mOutput << mName << ": no-snap\n";
          } else {
            mUseVectorOffset = mVectorOffset;
            *mOutput << mName << ": snap\n";
          }
          break;
        } else if (cmd == RK_RIGHT_ARROW) { // -> right arrow 301
          mUseVectorOffset -= mUnitSize;
          if (mUseVectorOffset < 0) mUseVectorOffset = 0;
          break;
        } else if (cmd == RK_LEFT_ARROW) { // <- left arrow  302
          mUseVectorOffset += mUnitSize;
          if (mUseVectorOffset > mBufferSize - mVectorSize) mUseVectorOffset = mBufferSize - mVectorSize;
          break;
        } else {
          // any character: save and continue
          store_data();
        }
        // no break
      case ReadyToRecord:
        if (cmd == '\n')
          enter(Label);
        else if (cmd == 'l') {
          if (!do_learn()) return;
          enter(Label);
        } else {
          prepare_class_for_recording(cmd);
          enter(CountDownReady);
          bang_me_in(countdown_time);
          send_note(60 + (mClassLabel % 12),80,100,1,0,2);
        }  
        break;
      case Label:
        if (cmd == (int)'r') {
          enter(ReadyToRecord);
          mClassLabel = 0;
          return;
        } else if (cmd == (int)'l') {
          if (!do_learn()) return;
          // no need to load model from file, it's in memory
        }
      }
    }
    
    
    // send mean value
    if (mMeanVector) {
      mS.array.value = mMeanVector;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 4);
    }
    
    if (mState == Validation) {
      
      // recorded signal
      mS.array.value = mBuffer + mUseVectorOffset;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 3);
    } else if (mState == Label) {
        bool change = predict();
        
        // send probabilities (map to mUnitSize first)
        mS.array.value = mVotes;
        mS.array.size  = mUnitSize;
        mS.type  = ArraySignal;
        send(mS, 5);
        if (mDebug) {
          for(int i = 0; i< mLabelCount; i++)
            *mOutput << "  " << mLabels[i];
          *mOutput << std::endl << mS << std::endl;
        }

        // live signal
        mS.array.value = mLiveBuffer + mLiveBufferSize - mVectorSize;
        mS.array.size  = mVectorSize;
        mS.type  = ArraySignal;
        send(mS, 3);
        
        if (change) send(mClassLabel);
    } else {

      // live signal
      mS.array.value = mLiveBuffer + mLiveBufferSize - mVectorSize;
      mS.array.size  = mVectorSize;
      mS.type  = ArraySignal;
      send(mS, 3);
    }
    
  }
  
  
  /** Accessor from command line. */
  void learn()
  {
    if (!do_learn()) {
      *mOutput << mName << ": learn failed.\n";
    }
  }
  
  
private:
  
  
  /** Transform the raw data into the sparse format used by libsvm. Use mThreshold to fix what is considered as zero. This outputs the file 'svm.train' containing all data with labels from the classes in mFolder. */
  void build()
  {
    if (mTrainFile) fclose(mTrainFile);
    mTrainFile = fopen(train_file_path().c_str(), "wb");
      if (!mTrainFile) {
        *mOutput << mName << ": could not open '" << train_file_path() << "' to store training data.\n";
        return;
      }
      
      DIR * directory;
      struct dirent *elem;

      directory = opendir(mFolder.c_str());
        if (directory == NULL) {
          *mOutput << mName << ": could not open directory '" << mFolder << "' to read class data.\n";
          return;
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
          label = filename.substr(start+6, end - start - 6);
          load_class(atoi(label.c_str()), &Svm::write_as_sparse);
        }
      closedir(directory);
        
    fclose(mTrainFile);
  }
  
  /** Update the model based on the current recorded samples. */
  bool do_learn()
  {
    build();
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
    
    return true;
  }
  
  
  bool predict()
  {
    int label;
    
    // map current vector into svm_node
    int i,pos,j = 0; // svm_node index
    double * vector = mLiveBuffer + mLiveBufferSize - mVectorSize;
    
    for(int i=0;i < mVectorSize; i++) {
      if (vector[i]) {
        mNode[j].index = i;
        mNode[j].value = vector[i];
        j++;
      }
    }
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
      mLabelProbability = (double)mVotes[vote_max_idx] / (double)(mLabelCount - 1);
		} else {
      label = svm_predict(mModel,mNode);
      mLabelProbability = 1.0;
    }
    
    if (label != mClassLabel) {
      mSameLabelCount = 1;
      mClassLabel = label;
      return true;
    } else {
      mSameLabelCount++;
    }
    return false;
  }
  
  
  void receive_data()
  {
    double * vector;
    if (!mLiveBuffer) {
      *mOutput << mName << ": no data to record (nothing coming from inlet 2).\n";
      mBuffer = NULL;
      return;
    }
    
    // copy data into our local buffer
    if (mThreshold) {
      for(int i=0; i < mBufferSize; i++) {
        if (mLiveBuffer[i] >= mThreshold || mLiveBuffer[i] <= -mThreshold) {
          mBuffer[i] = mLiveBuffer[i];
        } else
          mBuffer[i] = 0.0;
      }
    } else
      memcpy(mBuffer, mLiveBuffer + mLiveBufferSize - mBufferSize, mBufferSize * sizeof(double));
    
    if (mVectorCount) {
      // try to find the best bet by calculating minimal distance
      double distance, min_distance = -1.0;
      double d;
      int   delta_used = mBufferSize - mVectorSize;
      for(int j = mBufferSize - mVectorSize; j >= 0; j -= mUnitSize) {
        distance = 0.0;
        vector = mBuffer + j;
        for(int i=0; i < mVectorSize; i++) {
          d = (mMeanVector[i] - vector[i]) ;
          if (d > 0)
            distance += d;
          else
            distance -= d;
        }
        distance = distance / mVectorSize;
        if (min_distance < 0 || distance < min_distance) {
          delta_used = j;
          min_distance = distance;
        }
      }
      mVectorOffset = delta_used;
      bprint(mBuf,mBufSize, ": distance to mean vector %.3f (delta %i/%i)\n~> Keep ? ", min_distance, delta_used, mBufferSize - mVectorSize - delta_used);
      *mOutput << mName << mBuf << std::endl;
    } else {
      mVectorOffset = mBufferSize - mVectorSize * (1 + mMargin/2.0);
      *mOutput << mName << ":~> Keep ? " << std::endl;
    }
    mUseVectorOffset = mVectorOffset;
  }
  
  void store_data()
  {
    double * vector = mBuffer + mUseVectorOffset;
    
    if (!mBuffer) {
      *mOutput << mName << "(error): could not save data (empty buffer)\n";
      return;
    }
    // 1. write to file
    FILE * file = fopen(mClassFile.c_str(), "ab");
      if (!file) {
        *mOutput << mName << "(error): could not write to '" << mClassFile << "' (" << strerror(errno) << ")\n";
        return;
      }
      for(int i=0; i< mVectorSize; i++) {
        fprintf(file, "% .5f", vector[i]);
        if ((i+1)%mUnitSize == 0)
          fprintf(file, "\n");
        else
          fprintf(file, " ");
      }  
      fprintf(file, "\n");  // two \n\n between vectors
    fclose(file);
    // 2. update mean value
    update_mean_value(vector);
  }
  
  void prepare_class_for_recording(int cmd)
  {
    // record character as class id
    if (mClassLabel != cmd)
    {
      // new class
      load_class(cmd, &Svm::update_mean_value);
    }
    if ((cmd <= 'z' && cmd >= 'a') || (cmd <= 'Z' && cmd >= 'A') || (cmd <= '9' && cmd >= '0'))
      *mOutput << mName << ": recording vector " << mVectorCount + 1 << " for '" << (char)cmd << "'\n";
    else
      *mOutput << mName << ": recording vector " << mVectorCount + 1 << " for " << cmd << "\n";
  }
  
  void enter(svm_states_t pState)
  {
    switch(pState) {
    case ReadyToRecord:
      mState = pState;
      *mOutput << mName << ": Ready to record\n~> ";
      break;
    case Label: 
      if (mReadyToLabel) {
        *mOutput << mName << ": Ready to label.\n";
        mState = pState;
      } else {
        *mOutput << mName << ": Cannot enter 'label' mode.\n";
        enter(ReadyToRecord);
      }
      break;
    default:
      mState = pState;
    }
  }
  
  /** Update the mean value with the current vector. */
  void update_mean_value(double* vector)
  {
    mVectorCount++;
    double map = (double)(mVectorCount - 1) / (double)(mVectorCount);
    for(int i=0; i < mVectorSize; i++) {
      mMeanVector[i] = (mMeanVector[i] * map) + ( vector[i] / (double)mVectorCount );
    }
  }
  
  /** Execute the function for each vector contained in the class. */
  void load_class(int cmd, void (Svm::*function)(double*))
  {
    mClassLabel = cmd;
    /** reset mean value. */
    mVectorCount = 0;
    for(int i=0; i < mVectorSize; i++) mMeanVector[i] = 0.0;
    
    // 1. find file
    mClassFile = mFolder;
    bprint(mBuf, mBufSize, "/class_%i.txt", cmd);
    mClassFile.append(mBuf);
    
    // 2. open
    FILE * file;
    float val;
    int    value_count = 0;
    file = fopen(mClassFile.c_str(), "rb");
      if (!file) {
        *mOutput << mName << ": new class\n";
        //*mOutput << mName << "(error): could not read from '" << mClassFile << "' (" << strerror(errno) << ")\n"
        return;
      }
      // read a vector
      while(fscanf(file, " %f", &val) > 0) {
        fscanf(file, "\n"); // ignore newline
        mBuffer[value_count] = (double)val;
        if (value_count >= mVectorSize - 1) {
          // got one vector
          (this->*function)(mBuffer);
          value_count = 0;
        } else
          value_count++;
      }
    fclose(file);
  }
  
  void write_as_sparse (double * vector)
  {
    mVectorCount++;
    if (!mTrainFile) return;
    fprintf(mTrainFile, "%+i", mClassLabel);
    for(int i=0; i < mVectorSize; i++) {
      if (vector[i]) {
        fprintf(mTrainFile, " %i:%.5f", i+1, (float)vector[i]);
      }
    }
    fprintf(mTrainFile, "\n");
  }
  
  void load_model ()
  { 
    if (mModel) svm_destroy_model(mModel);
    mModel = svm_load_model(model_file_path().c_str());
    if (!mModel) {
      *mOutput << mName << ": could not load model file '" << model_file_path() << "'.\n";
      return;
    }
    
    // prepare probability / labels buffer
    mLabelCount = svm_get_nr_class(mModel);
    
    mLabels         = (int*)    malloc(mLabelCount * sizeof(int));
    if (!mLabels) {
      *mOutput << mName << ": could not allocate " << mLabelCount << " ints for labels.\n";
      return;
    }
    svm_get_labels(mModel, mLabels);
    
		mDistances     = (double*) malloc((mLabelCount * (mLabelCount-1)/2) * sizeof(double));
    if (!mDistances) {
      *mOutput << mName << ": could not allocate " << (mLabelCount * (mLabelCount-1)/2) << " doubles for distances.\n";
      return;
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
      return;
    }
    for(int i=0;i<vote_count;i++) mVotes[i] = 0.0;
    
    
    mReadyToLabel = true;
    enter(Label);
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

  svm_states_t mState;
  std::string mFolder; /**< Folder containing the class data. */
  std::string mClassFile; /**< Current class data file. */
  FILE * mTrainFile;

  bool mReadyToLabel; /**< Set to true when svm is up to date. */
  bool mReadyToLearn; /**< Set to true when there is data to learn from. */
  
  int mCountDown;
  int mVectorOffset;     /**< Best match with this offset in mBuffer. */
  int mUseVectorOffset;  /**< Offset to use. */
  double * mMeanVector; /**< Store the mean value for all vectors from this class. */
  double * mLiveBuffer; /**< Pointer to the current buffer window. Content can change between calls. */
  double * mBuffer;     /**< Store a single vector +  margin. */
  double   mMargin;     /**< Size (in %) of the margin. */
  int mVectorSize;
  int mUnitSize;       /**< How many values form a sample (single event). */
  int mVectorCount;    /**< Number of vectors used to build the current mean value. */
  int mSampleRate; /**< How many samples per second do we receive from the 'data' inlet. */
  int mBufferSize; /**< Size of buffered data ( = mVectorSize + 25%). We use more then the vector size to find the best fit. */
  int mLiveBufferSize;
  int mClassLabel; /**< Current label. Used during recording and recognition. */
  double mLabelProbability; /**< Current probability for the given label. */
  int mSameLabelCount; /**< Count for how long the current label has been set. */
  
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
  OUTLET(Svm,countdown)
  OUTLET(Svm,current)
  OUTLET(Svm,mean)
  OUTLET(Svm,probabilities)
  METHOD(Svm,learn)
}