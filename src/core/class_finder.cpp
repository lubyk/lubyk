/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "class_finder.h"
#include "planet.h"
#include <dlfcn.h> // dylib load


/** This trigger implements "/class". It returns the list of objects in objects_path_. */
const Value ClassFinder::trigger (const Value &val)
{
  return gNilValue; // TODO: 'lib' directory listing ?
}

bool ClassFinder::build_child(const std::string &class_name, const Value &type, Value *error, ObjectHandle *object) {
  std::cout << "Trying to load " << objects_path_ << "/" << class_name << "\n";
  // try to load dynamic lib
  std::string path = objects_path_;
  path.append("/").append(class_name).append(".rko");
  if (load(path.c_str(), "init")) {
    if (get_child(class_name, object)) {
      // Found object (everything went fine) !
      return true;
    } else {
      error->set(INTERNAL_SERVER_ERROR, std::string("'").append(path).append("' should declare '").append(class_name).append("'."));
      return false;
    }
  } else {
    error->set(INTERNAL_SERVER_ERROR, std::string("Could not load '").append(path).append("'."));
    return false;
  }
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool ClassFinder::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(Planet*);
  const char *error = 0;

  // load shared extension image into memory
  // --->
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if ( (error = dlerror()) )
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }

  // get 'init' function into the image
  function = (void(*)(Planet*))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    printf("Symbol '%s' not found in '%s'.",init_name,file);
    if ( (error = dlerror()) )
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }

  Planet *planet = TYPE_CAST(Planet, root_);

  // call 'init', passing planet
  if (planet) {
    (*function)(planet);
  } else {
    fprintf(stderr, "Could not cast root_ to Planet* !\n");
    return false;
  }

  return true;
}