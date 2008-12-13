#ifndef _PLANET_H_
#define _PLANET_H_
#include "group.h"
#include "root.h"

class Planet : public Group
{
public:
  bool init (const Value& p);
  
  virtual const Value inspect(const Value& val) ;
  
  virtual void bang(const Value& val);
  
  Root * tree()
  {
    return &mTree;
  }
  
protected:
  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void do_adopt(Object * pObject)
  {
    mTree.do_adopt(pObject);
  }
private:
  Root mTree;
};

#endif // _PLANET_H_