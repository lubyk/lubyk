#include "event.h"
#include "node.h"

void BangEvent::cast_bang_method (Node *receiver, const Value &parameter) {
  receiver->bang(parameter);
}