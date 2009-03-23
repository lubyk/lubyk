#include "oscit/root.h"

namespace oscit {
Root::~Root()
{
  std::list<OscSend*>::iterator it  = mControllers.begin();
  std::list<OscSend*>::iterator end = mControllers.end();

  while (it != end) delete *it++;
  
  if (mOscIn) delete mOscIn;
  clear();
  root_ = NULL; // avoid call to unregisterObject in ~Object
}
} // namespace oscit