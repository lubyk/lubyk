#ifndef _NEW_METHOD_H_
#define _NEW_METHOD_H_
#include "planet.h"
#include "class.h"

#define NEW_METHOD_INFO "Create new objects from a given url and a hash of parameters."

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
    Object *parent;
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
    if (url.at(0) == '/') {
      size_t pos = url.rfind("/");
      parent = planet->object_at( url.substr(0, pos) );
      if (!parent) return ErrorValue(BAD_REQUEST_ERROR, "Invalid parent '").append(url.substr(0, pos)).append("'.");
      name = url.substr(pos + 1, url.length());
    } else {
      parent = NULL;
      name = url;
    }
    
    T * node;
    
    if (parent)
      node = parent->adopt(new T());
    else
      node = planet->adopt(new T());
    
    node->set_name(name);
    
    Class * klass = TYPE_CAST(Class, new_method->parent_); // the parent of 'new' should be a Class.

    if (!klass) {
      return ErrorValue(INTERNAL_SERVER_ERROR, "Object at '").append(new_method->parent_->url()).append("' is not a Class !");
    }
    
    // build methods from prototype
    klass->make_methods(node);
    

    node->set_class_url(new_method->parent_->url());  // used by osc (using url instead of name because we might have class folders/subfolders some day).

    // make inlets
    klass->make_inlets(node);

    // make outlets
    klass->make_outlets(node);
    
    if (!params.is_nil()) {
      // set defaults by calling methods
      node->trigger(params);
      // if init returns false, the node goes into 'broken' mode.
      node->set_is_ok(node->init());
    } else {
      node->set_is_ok(node->init());
    }
    
    return Value(node->url());
  }
 private:
  create_method_t class_method_;
  
};

#endif // _NEW_METHOD_H_