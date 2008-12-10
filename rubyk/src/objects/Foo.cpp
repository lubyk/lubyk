#include "rubyk.h"

class Foo : public Node
{
  // default inlet
  void bang (const Value& val)
  {
    Matrix m;
    if (!sig.set(&m)) return; // light copy (shares memory with "sig")
    m *= mMult;     // real copy (with multiplication)
    send(1,m);      // "m" is automatically deallocated (this is an example: in a real object, 'm' would be a member so that it is reused.)
  }

  // get mMult parameter
  const Value& multiplier ()
  {
    return Number(mMult);
  }

  // inlet 2: set mMult (also used as param)
  void multiplier (const Value& val)
  {
    sig.set(mMult);
  }
  
  // These two methods could be abstracted with
  // ATTR_READER(multiplier, Number(mMult))
  // and
  // ATTR_WRITER(multiplier, mMult)
  // or
  // ATTR_ACCESSOR(multiplier, Number(mMult), mMult)
  // or
  // NUMBER_ACCESSOR(multiplier, mMult)
  
  
private:
  real_t mMult;
}

extern "C" void init()
{  
  // define class
  CLASS (   Foo, "Matrix multiplication.")
  
  // define bang  (class, method, accepted types, message)
  INLET (   Foo, bang,       NumberValue | MatrixValue, "Multiply by right inlet." )
  
  // define multiplier inlet (also creates "/nodes/foo/multiplier" set/get methods and "nodes/foo/in/multiplier" to link, unlink, ...)
  INLET (   Foo, multiplier, NumberValue | MatrixValue, "Multiply left inlet." )
  
  // define an outlet (also creates "/nodes/foo/out/result" to link, unlink, etc)
  // to "spy" on the outlet's result by hovering connection/outlet, we must register as observer on the outlet (=osc connection).
  OUTLET(   Foo, result,     MatrixValue,               "Multiplication result.")
  
  // define accessors (if there is no inlet)
  ACCESSOR( ClassRecorder, tempo, NumberValue, "Ready, set, go tempo used when recording (0 = right away)." )
  
  // define instance method
  //      class  method      parameter type           return type    info message
  METHOD( Metro, start_stop, NumberValue | BangValue, NumberValue,   "If the input value is 0: stop. If it is greater the 0: start. Bang toggles on/off." )
  
  // define class method
  //            class   static method   parameter type  return type  info message
  CLASS_METHOD( MidiIn, ports,          NilValue,       StringValue, "If the input value is 0: stop. If it is greater the 0: start. Bang toggles on/off." )
  
  
}