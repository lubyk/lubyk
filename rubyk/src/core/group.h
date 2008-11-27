#ifndef _GROUP_H_
#define _GROUP_H_
#include "node.h"

class Group : public Node
{
public:
  virtual bool init (const Value& p);
  
  virtual bool set (const Value& p);

  virtual void spy();
  
  virtual void bang(const Value& sig);
};
#endif