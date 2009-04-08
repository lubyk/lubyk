#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "oscit.h"

class Planet;

/** Object is the base object in Rubyk context. The object is just a wrapper around oscit::BaseObject
 *  with an extra "Planet" pointer. */
class Object : public BaseObject
{
public:
  virtual void set_root(Root * root)
  {  
    Object::set_root(root);
    if (root) planet_ = (Planet*)(root->ground_);
  }
  
protected:
  
  Planet * ground_;  /**< Planet on which the objects will live. */
};

#endif // _OBJECT_H_