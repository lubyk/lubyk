#include "rubyk.h"

const Value NewMethod::trigger (const Value& val)
{
  String url;
  Value params;
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
  
  Object * obj = mRoot->find(url.string());
  
  if (!obj) {
    return Error("New Object at '").append(url.string()).append("' not found !");
  }
  
  Class * klass = TYPE_CAST(Class,mParent);
  
  if (!klass) {
    return Error("Object at '").append(mParent->url()).append("' is not a Class !");
  }
  
  // make methods (must happen first so that the first child called as anonymous "set" method is a method)
  klass->make_methods(obj);
  
  Node * node = TYPE_CAST(Node,obj);
  
  if (!node) {
    // not a Node. Call methods with "set" and finish.
    obj->set(params);
    return url;
  }
  
  // Node specific initializations
  
  node->set_class_url(mParent->url());  // used by osc (using url instead of name because we might have class folders/subfolders some day).
  
  // make inlets
  klass->make_inlets(node);
  
  // make outlets
  klass->make_outlets(node);
  
  // initialize
  node->set_is_ok( node->init() && node->set(params) ); // if init or set returns false, the node goes into 'broken' mode.
  
  // create pending links
  mRoot->create_pending_links();
  
  return url;
}
