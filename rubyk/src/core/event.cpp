#include "event.h"
#include "node.h"


void BangEvent::cast_bang_method (void * pReceiver)
{
  ((Node*)pReceiver)->bang(gBangValue);
}