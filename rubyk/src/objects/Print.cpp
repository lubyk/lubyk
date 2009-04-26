#include "rubyk.h"
#include "print.h"

extern "C" void init(Planet &planet) {
  CLASS(Print, "Print any value received in bang inlet.", "no options")
  // [1] print
  INLET(Print, print, AnyIO("Received values are printed out."))
  // {1} prefix
  ADD_METHOD(Print, "prefix", prefix, StringIO("any", "Prefix to print before values."))
  // {2} print
  ADD_METHOD(Print, "print",  print,  AnyIO("Received values are printed out."))
  // [2] prefix
  INLET(Print, prefix, StringIO("any", "Prefix to print before values."))
}