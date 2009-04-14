#ifndef _NEW_METHOD_H_
#define _NEW_METHOD_H_
#include "planet.h"

#define NEW_METHOD_INFO "Create new objects from a given url and a hash of parameters."

class NewMethod : public ClassMethod
{
public:
  NewMethod(const std::string &name, class_method_t method, const char *info) : ClassMethod(name, method, H("sH"), info) {}
  NewMethod(const char *name, class_method_t method, const char *info) : ClassMethod(name, method, H("sH"), info) {}
  virtual ~NewMethod() {}
  
  /** This trigger method actually implements "new". The parameter can be either a String containing the url of the new object or
    * a Hash containing the "url" and "params": {url: "foo" params:{ bar:1 pico:2 }}. */
  virtual const Value trigger (const Value &val);
  
  /** Template used as a method to create new nodes. */
  template<class T>
  static const Value cast_create(Planet &planet, const Value &val)
  {
    if (val.is_nil()) return gNilValue; // ??? any idea for something better here ?
    Object *parent;
    Value params(val[1]);
    std::string url(val[0].str());
    std::string name;
    
    // get parent ["/met", "met"] => "", "/grp/met" => "/grp"
    if (url.at(0) == '/') {
      size_t pos = url.rfind("/");
      parent = planet.object_at( url.substr(0, pos) );
      if (!parent) return ErrorValue(BAD_REQUEST_ERROR, "Invalid parent '").append(url.substr(0, pos)).append("'.");
      name = url.substr(pos + 1, url.length());
    } else {
      parent = NULL;
      name = str;
    }
    
    T * obj;
    
    if (parent)
      obj = parent->adopt(new T());
    else
      obj = planet.adopt(new T());
    
    obj->set_name(name);
    obj->set(val[1]);
    
    return String(obj->url());
  }
  
};

#endif // _NEW_METHOD_H_