#ifndef _NEW_METHOD_H_
#define _NEW_METHOD_H_
#include "method.h"
#include "planet.h"

class NewMethod : public ClassMethod
{
public:
  NewMethod(const std::string &pName, class_method_t pMethod) : ClassMethod(pName, pMethod) {}
  NewMethod(const char * pName, class_method_t pMethod) : ClassMethod(pName, pMethod) {}
  virtual ~NewMethod() {}
  
  /** This trigger method actually implements "new". The parameter can be either a String containing the url of the new object or
    * a Hash containing the "url" and "params": {url: "foo" params:{ bar:1 pico:2 }}. */
  virtual const Value trigger (const Value val);
  
  /** Template used as a method to create new nodes. */
  template<class T>
  static const Value cast_create(Planet& pPlanet, const Value val)
  {
    oscit::Object * parent;
    String url(val);
    std::string name;
    if (url.is_nil()) return Error("Invalid 'url' parameter.");
    
    // get parent ["/met", "met"] => "", "/grp/met" => "/grp"
    std::string str = url.string();
    if (str.at(0) == '/') {
      size_t pos = url.rfind("/");
      parent = pPlanet.find( url.substr(0, pos) );
      if (!parent) return Error("Invalid parent '").append(url.substr(0,pos)).append("'.");
      name = url.substr(pos + 1, url.length());
    } else {
      parent = NULL;
      name = str;
    }
    
    T * obj;
    
    if (parent)
      obj = parent->adopt(new T());
    else
      obj = pPlanet.root()->adopt(new T());
    
    obj->set_name(name);
    
    return String(obj->url());
  }
  
};

#endif // _NEW_METHOD_H_