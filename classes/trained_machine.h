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
  bool set_machine(const Params& p)
  {
    std::string str;
    if (p.get(&str, "data"))
      mFolder = str;
    else if (mFolder == "")
      mFolder = "data";
    
    size_t vector_size = p.val("vector", mVector.col_count());
    if (vector_size <= 0) return mVector.set_sizes(1, 32);
    else return mVector.set_sizes(1, vector_size);
  }

protected:
  std::string mFolder; /**< Folder containing the class data. */
  Matrix   mVector;    /**< Temporary vector used to set/get from files. */
  
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