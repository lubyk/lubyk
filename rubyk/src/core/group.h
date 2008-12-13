#ifndef _GROUP_H_
#define _GROUP_H_
#include "root.h"
#include "node.h"

class Group : public Node
{
public:
  bool init (const Value& p);
  
  virtual const Value inspect(const Value& val) ;
  
  virtual void bang(const Value& val);
  
};
#endif