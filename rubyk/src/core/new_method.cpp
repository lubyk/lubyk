#include "rubyk.h"

const Value NewMethod::trigger (const Value& val)
{
  String url;
  Hash params;
  Value res;
  if (val.is_hash()) {
    url    = Hash(val)["url"];
    params = Hash(val)["params"];
  } else {
    url = val;
  }
  
  if (url.is_nil()) return Error("Invalid parameters (missing 'url').");
  
  res = (*mMethod)(*mRoot, url);
  
  if (!res.is_string()) return res;
  url = res;
  
  Node * obj = TYPE_CAST(Node,mRoot->find(url.string()));
  
  if (!obj) {
    return Error("New Node at '").append(url.string()).append("' not found !");
  }
  
  obj->set_class_url(mParent->url());  // used by osc (using url instead of name because we might have class folders/subfolders some day).
  
  //FIX obj->set_server(pServer);  // ??? where do we find this one ?
  //FIX obj->set_output(pOutput);  // ??? I think we won't need this anymore once we have implemented notifications ?
  
  Class * klass = TYPE_CAST(Class,mParent);
  
  // make inlets
  klass->make_inlets(obj);
  
  // make outlets
  klass->make_outlets(obj);
  
  // make methods
  klass->make_methods(obj);
  
  // initialize
  obj->set_is_ok( obj->init() && obj->set(params) ); // if init or set returns false, the node goes into 'broken' mode.
  return url;
}
