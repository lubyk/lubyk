#include "event.h"
#include "node.h"


void BangEvent::cast_bang_me_inthod (void *receiver, const Value &parameter) {
  ((Node*)receiver)->bang(gNilValue);
}