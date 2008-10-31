#include "node.h"

class Group : public Node
{
public:
  virtual bool init (const Params& p);
  
  virtual bool set (const Params& p);

  virtual void spy();
  
  virtual void bang(const Signal& sig);
};