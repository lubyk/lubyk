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

#ifndef RUBYK_INCLUDE_RUBYK_NEW_METHOD_H_
#define RUBYK_INCLUDE_RUBYK_NEW_METHOD_H_

#include "rubyk/planet.h"
#include "rubyk/class.h"

#define NEW_METHOD_INFO "Create new objects from a given url and a hash of parameters."

namespace rk {

class NewMethod;
typedef const Value (*create_method_t)(Planet *root, NewMethod *new_method, const Value &val);

class NewMethod : public Object
{
public:
  TYPED("Object.NewMethod")

  NewMethod(const std::string &name, create_method_t method, const Value &type) : Object(name, type), class_method_(method) {}

  NewMethod(const char *name, create_method_t method, const Value &type) : Object(name, type), class_method_(method) {}

  virtual ~NewMethod() {}

  /** This trigger method actually implements "new".
   *  @param val should be a string (url) followed by a hash (parameters): ["foo", {metro:115 rubato:0.7}].
   */
  virtual const Value trigger (const Value &val) {
    return (*class_method_)((Planet*)root_, this, val);
  }

  /** Template used as a method to create new nodes. */
  template<class T>
  static const Value cast_create(Planet *planet, NewMethod *new_method, const Value &val) {
    if (val.is_nil()) return gNilValue; // ??? any idea for something better here ?
    ObjectHandle parent;
    Value params;
    std::string url;
    std::string name;

    if (val.first().is_string()) {
      url = val.first().str();
    } else {
      return ErrorValue(BAD_REQUEST_ERROR, "Invalid arguments. First argument shoul be an url (type 's').");
    }

    if (val.size() > 1) {
      params = val[1];
    } else {
      params.set_nil();
    }

    // get parent ["/met", "met"] => "", "/grp/met" => "/grp"
    if (url.size() > 0 && url.at(0) == '/') {
      size_t pos = url.rfind("/");
      if (!planet->get_object_at(url.substr(0, pos), &parent)) return ErrorValue(BAD_REQUEST_ERROR, "Invalid parent '").append(url.substr(0, pos)).append("'.");
      name = url.substr(pos + 1, url.length());
    } else {
      name = url;
    }

    T *node;

    if (parent.ptr()) {
      node = parent->adopt(new T());
    } else {
      node = planet->adopt(new T());
    }

    node->set_name(name);

    Class * klass = TYPE_CAST(Class, new_method->parent_); // the parent of 'new' should be a Class.

    if (!klass) {
      return ErrorValue(INTERNAL_SERVER_ERROR, "Object at '").append(new_method->parent_->url()).append("' is not a Class !");
    }

    // build methods from prototype
    klass->make_methods(node);

    // used by osc (using url instead of name because we might have class folders/subfolders some day).
    // not very clear why we need this...
    node->set_class_url(Value(new_method->parent_->url()));

    // make outlets
    klass->make_outlets(node);

    Value res = node->init();

    if (!res.is_error() && !params.is_nil()) {
      // set by calling methods
      if (params.is_hash()) {
        res = node->set(params);
      } else {
        // trigger first method
        ObjectHandle handle;
        if (node->first_child(&handle)) {
          res = handle->trigger(params);
        }
      }
    }

    if (res.contains_error()) {
      node->set_is_ok(false);
      return res;
    }

    res = node->start();
    if (res.contains_error()) {
      node->set_is_ok(false);
      return res;
    }

    return Value(node->url());
  }
 private:
  create_method_t class_method_;

};

} // rk

#endif // RUBYK_INCLUDE_RUBYK_NEW_METHOD_H_