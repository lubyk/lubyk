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

#include "rubyk/class_finder.h"
#include "rubyk/planet.h"
#include <dlfcn.h> // dylib load

namespace rk {

/** This trigger implements "/class". It returns the list of objects in objects_path_. */
const Value ClassFinder::trigger (const Value &val)
{
  return gNilValue; // TODO: 'lib' directory listing ?
}

bool ClassFinder::build_child(const std::string &class_name, const Value &type, Value *error, ObjectHandle *object) {
  // try to load dynamic lib
  // FIXME: support rk patches and/or Lua ==> Lua(...)

  size_t paths_count = search_paths_.size();
  for (size_t i = 0; i < paths_count; ++i) {

    std::string path = search_paths_[i].str();
    // FIXME: resolve "~"
    path.append("/").append(class_name).append(".rko");
    if (load(path.c_str(), "init", error)) {
      if (get_child(class_name, object)) {
        // Found object (everything went fine) !
        return true;
      } else {
        *error = FValue(INTERNAL_SERVER_ERROR, "'%s' should declare '%s'.", path.c_str(), class_name.c_str());
        return false;
      }
    } else if (error->is_error()) {
      return false;
    }
    // continue searching
  }

  // not found
  *error = FValue(BAD_REQUEST_ERROR, "Could not find '%s' in '%s'.", class_name.c_str(), search_paths_.join(", ").c_str());
  return false;

}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool ClassFinder::load(const char * file, const char * init_name, Value *error)
{
  void *image;
  void (*function)(Planet*);

  // load shared extension image into memory
  // --->
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    // do not set error, just return false to continue searching
    return false;
  }

  // get 'init' function into the image
  function = (void(*)(Planet*))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    *error = FValue("Symbol '%s' not found in '%s'.", init_name, file);
    return false;
  }

  Planet *planet = TYPE_CAST(Planet, root_);

  // call 'init', passing planet
  if (planet) {
    (*function)(planet);
  } else {
    *error = Value(INTERNAL_SERVER_ERROR, "Could not cast root to Planet.");
    return false;
  }

  return true;
}

} // rk
