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
  bool init_machine(const Params& p)
  {
    mFolder     = p.val("data", std::string("data"));
    mVectorSize = p.val("vector", 32);
    return true;    
  }

protected:
  std::string mFolder; /**< Folder containing the class data. */
  int      mVectorSize;      /**< Number of samples per vector. */
  
  
  // FIXME: replace bool return values by exception handling...
  template <class T, bool(T::*Tmethod)(const std::string& filename, double * vector)>
  bool foreach_train_class()
  {
    DIR * directory = NULL;
    struct dirent *elem;
    double * vector = NULL;
    if (!alloc_doubles(&vector, mVectorSize, "temporary")) return false;
    directory = opendir(mFolder.c_str());
      if (directory == NULL) {
        *mOutput << mName << ": could not open directory '" << mFolder << "' to read class data.\n";
        goto foreach_train_class_fail;
      }
      while (elem = readdir(directory)) {
        std::string filename(elem->d_name);
        std::string path = std::string(mFolder).append("/").append(filename);
        int start, end;
        start = filename.find("class_");
        end   = filename.find(".txt");
        if (start == std::string::npos || end == std::string::npos) {
          // bad filename, skip
          continue;
        }
        
        if(!((((T*)this)->*Tmethod)(filename, NULL))) goto foreach_train_class_fail; // send NULL vector to clear
        FILE * file;
        float val;
        int    value_count = 0;
        file = fopen(path.c_str(), "rb");
          if (!file) {
            *mOutput << mName << "(error): could not read from '" << path << "' (" << strerror(errno) << ")\n";
            goto foreach_train_class_fail;
          }
          // read a vector
          while(fscanf(file, " %f", &val) != EOF) {
            fscanf(file, "\n"); // ignore newline
            vector[value_count] = (double)val;
            value_count++;
            if (value_count >= mVectorSize) {
              // got one vector
              if(!((((T*)this)->*Tmethod)(filename, vector))) goto foreach_train_class_fail;
              value_count = 0;
            }
          }
        fclose(file);
        if(!((((T*)this)->*Tmethod)(filename, NULL))) goto foreach_train_class_fail; // send NULL vector to inform class is finished
      }
    closedir(directory);
    
    free(vector);
    return true;
foreach_train_class_fail:
    *mOutput << mName << ": foreach_train_class callback return false.\n";
    if (vector) free(vector);
    if (directory) closedir(directory);
    return false;
  }
};

#define FOREACH_TRAIN_CLASS(klass, method) (foreach_train_class<klass, &klass::method>())

#endif // _TRAINED_MACHINE_H_