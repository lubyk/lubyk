#ifndef _TRAINED_MACHINE_H_
#define _TRAINED_MACHINE_H_

#include "class.h"

#include <errno.h>     // Error number definitions
#include <sys/types.h> // directory listing
#include <dirent.h>    // directory listing

class TrainedMachine : public Node
{
public:
  virtual ~TrainedMachine () {}
  
  bool init_machine(const Value& p)
  {
    TRY(mVector, set_sizes(1,8));
    return true;
  }
  
  bool set_machine(const Value& p)
  {
    std::string str;
    if (p.get(&str, "data"))
      mFolder = str;
    else if (mFolder == "")
      mFolder = "data";
    
    size_t vector_size = mVector.col_count();
    if (p.get(&vector_size, "vector")) {
      TRY(mVector, set_sizes(1, vector_size));
    }  
    return load_model();
  }
  
  /** Learn from new data. */
  virtual bool learn_from_data() = 0;
  
  /** Accessor from command line. */
  void learn()
  {
    if (!learn_from_data()) {
      *mOutput << mName << ": learn failed.\n";
      mIsOK = false;
    } else {
      mIsOK = load_model();
    }
  }
  
  /** Load model from file. */
  virtual bool load_model() = 0;
  
  void load()
  {
    if (!load_model()) {
      *mOutput << mName << ": could not load model.\n";
      mIsOK = false;
    } else {
      mIsOK = true;
    }
  }
protected:
  std::string mFolder; /**< Folder containing the class data. */
  Matrix   mVector;    /**< Temporary vector used to set/get from files. */
  
  bool get_label_from_filename(int * pRes, const std::string& pFilename)
  {
    size_t start = pFilename.find("class_");
    size_t end   = pFilename.find(".txt");
    if (start == std::string::npos || end == std::string::npos) {
      *mOutput << mName << ": bad filename '" << pFilename << "'. Should be 'class_[...].txt'.\n";
      return false;
    }
    *pRes = pFilename.substr(start+6, end - start - 6).c_str()[0];
    return true;
  }
  
  virtual std::string model_file_path()
  {
    std::string path(mFolder);
    path.append("/").append(mName).append(".model");
    return path;
  }
  
  template <class T, bool(T::*Tmethod)(const std::string& filename, Matrix * vector)>
  bool foreach_train_class()
  {
    DIR * directory = NULL;
    struct dirent *elem;
    directory = opendir(mFolder.c_str());
      if (directory == NULL) {
        *mOutput << mName << ": could not open directory '" << mFolder << "' to read class data.\n";
        goto foreach_train_class_fail;
      }
      while ( (elem = readdir(directory)) ) {
        std::string filename(elem->d_name);
        std::string path = std::string(mFolder).append("/").append(filename);
        size_t start, end;
        start = filename.find("class_");
        end   = filename.find(".txt");
        if (start == std::string::npos || end == std::string::npos) {
          // bad filename, skip
          continue;
        }
        
        if(!((((T*)this)->*Tmethod)(filename, NULL))) goto foreach_train_class_fail; // send NULL vector to clear
        
        FILE * file;
        file = fopen(path.c_str(), "rb");
          if (!file) {
            *mOutput << mName << "(error): could not read from '" << path << "' (" << strerror(errno) << ")\n";
            goto foreach_train_class_fail;
          }
          // read vectors one by one
          while(mVector.from_file(file))
            if(!((((T*)this)->*Tmethod)(filename, &mVector))) goto foreach_train_class_fail;
        fclose(file);
        if(!((((T*)this)->*Tmethod)(filename, NULL))) goto foreach_train_class_fail; // send NULL vector to inform class is finished
      }
    closedir(directory);
    return true;
foreach_train_class_fail:
    *mOutput << mName << ": foreach_train_class callback return false.\n";
    if (directory) closedir(directory);
    return false;
  }
};

#define FOREACH_TRAIN_CLASS(klass, method) (foreach_train_class<klass, &klass::method>())

#endif // _TRAINED_MACHINE_H_