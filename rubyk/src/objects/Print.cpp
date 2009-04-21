#include "rubyk.h"
#include "print.h"

extern "C" void init(Planet &planet) {
  CLASS(Print, "Print any value received in bang inlet.", "no options")
  METHOD(Print, print,  AnyIO("Received values are printed out."))
  METHOD(Print, prefix, StringIO("any", "Prefix to print before values."))
}