#ifndef _GROUP_H_
#define _GROUP_H_
#include "node.h"

class Group : public Node
{
public:
  virtual ~Group() {}
  
  bool init (const Value& p);
  
  virtual const Value inspect(const Value val) ;
  
  virtual void bang(const Value val);
  
};

#endif // _GROUP_H_