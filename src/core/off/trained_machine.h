/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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
  
  bool init_machine(const Value &p)
  {
    TRY(mVector, set_sizes(1,8));
    return true;
  }
  
  bool set_machine(const Value &p)
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
      *output_ << name_ << ": learn failed.\n";
      is_ok_ = false;
    } else {
      is_ok_ = load_model();
    }
  }
  
  /** Load model from file. */
  virtual bool load_model() = 0;
  
  void load()
  {
    if (!load_model()) {
      *output_ << name_ << ": could not load model.\n";
      is_ok_ = false;
    } else {
      is_ok_ = true;
    }
  }
protected:
  std::string mFolder; /**< Folder containing the class data. */
  Matrix   mVector;    /**< Temporary vector used to set/get from files. */
  
  bool get_label_from_filename(int * pRes, const std::string &pFilename)
  {
    size_t start = pFilename.find("class_");
    size_t end   = pFilename.find(".txt");
    if (start == std::string::npos || end == std::string::npos) {
      *output_ << name_ << ": bad filename '" << pFilename << "'. Should be 'class_[...].txt'.\n";
      return false;
    }
    *pRes = pFilename.substr(start+6, end - start - 6).c_str()[0];
    return true;
  }
  
  virtual std::string model_file_path()
  {
    std::string path(mFolder);
    path.append("/").append(name_).append(".model");
    return path;
  }
  
  template <class T, bool(T::*Tmethod)(const std::string &filename, Matrix * vector)>
  bool foreach_train_class()
  {
    DIR * directory = NULL;
    struct dirent *elem;
    directory = opendir(mFolder.c_str());
      if (directory == NULL) {
        *output_ << name_ << ": could not open directory '" << mFolder << "' to read class data.\n";
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
            *output_ << name_ << "(error): could not read from '" << path << "' (" << strerror(errno) << ")\n";
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
    *output_ << name_ << ": foreach_train_class callback return false.\n";
    if (directory) closedir(directory);
    return false;
  }
};

#define FOREACH_TRAIN_CLASS(klass, method) (foreach_train_class<klass, &klass::method>())

#endif // _TRAINED_MACHINE_H_