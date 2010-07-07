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

#ifndef RUBYK_INCLUDE_RUBYK_CLASS_FINDER_H_
#define RUBYK_INCLUDE_RUBYK_CLASS_FINDER_H_

#include "rubyk/oscit.h"
#include "rubyk/new_method.h"
#include "rubyk/class.h"

namespace rk {

/** Special class to handle class listing and loading from a set of directory paths.
 * This usually responds at to the '/class' url.
 */
class ClassFinder : public Object
{
public:
  TYPED("Object.ClassFinder")

  ClassFinder(const std::string &name, const char *objects_path) : Object(name) {
    lib_path(Value(objects_path));
    init();
  }

  ClassFinder(const std::string &name, std::string &objects_path) : Object(name) {
    lib_path(Value(objects_path));
    init();
  }

  ClassFinder(const char *name, const char *objects_path) : Object(name) {
    lib_path(Value(objects_path));
    init();
  }

  ClassFinder(const char *name, std::string &objects_path) : Object(name) {
    lib_path(Value(objects_path));
    init();
  }

  void init() {
    //          /class/lib
    adopt(new TMethod<ClassFinder, &ClassFinder::lib_path>(this, Url(LIB_URL).name(), Attribute::string_io("File path to load classes (*.rko, lua, etc).")));
  }

  virtual ~ClassFinder() {}

  /** This trigger implements "/class". It returns the list of objects in objects_path_. */
  virtual const Value trigger (const Value &val);

  virtual bool build_child(const std::string &class_name, const Value &type, Value *error, ObjectHandle *object);

  /** Takes a list of double dot separated strings as search paths.
   */
  const Value lib_path(const Value &val) {
    if (val.is_string()) {
      search_paths_ = val.split(":");
    }
    return search_paths_.join(":");
  }

  /** Declare a new class. This template is responsible for generating the "new" method.
   * FIXME: use ObjectHandle instead of class pointer.
   */
  template<class T>
  Class *declare(const char *name, const char *info, const char *options) {
    Class *klass;
    ObjectHandle class_object;

    if (find_class(name, &class_object) && (klass = class_object.type_cast<Class>()) ) {
      // remove existing class with same name.
      klass->release();
      class_object = NULL;
    }

    klass = adopt(new Class(name, Attribute::no_io(info)));

    if (!klass) return NULL; // FIXME: this will crash !!!

    // build "new" method for this class
    std::string info_string("Create a new ");
    info_string.append(name).append(" from a given url and optional Hash of parameters (").append(options).append(").");

    klass->adopt(new NewMethod( "new", &NewMethod::cast_create<T>, Attribute::any_io(info_string.c_str())));

    return klass;
  }

  /** Get a Class object from it's name ("Metro"). */
  bool find_class(const char *name, ObjectHandle *object) {
    return get_child(name, object);
  }

  /** Get a Class object from it's std::string name ("Metro"). */
  bool find_class(const std::string &name, ObjectHandle *object) {
    return get_child(name, object);
  }

private:
  /** Load an object stored in a dynamic library. */
  bool load(const char * file, const char * init_name, Value *error);

  /** A list of paths to search for objects in the filesystem.
   */
  Value search_paths_;
};

} // rk

#endif // RUBYK_INCLUDE_RUBYK_CLASS_FINDER_H_