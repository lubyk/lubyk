#ifndef _GROUP_H_
#define _GROUP_H_
#include "node.h"

class Group : public Node
{
public:
  virtual bool init (const Params& p);
  
  virtual bool set (const Params& p);

  virtual void spy();
  
  virtual void bang(const Signal& sig);
};
#endif