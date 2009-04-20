#include "rubyk.h"
#include "print.h"

extern "C" void init(Planet &planet) {
  CLASS(Print, "Print any value received in bang inlet.", "no options")
  INLET(Print, bang, AnyIO("Received values are printed out."))
}