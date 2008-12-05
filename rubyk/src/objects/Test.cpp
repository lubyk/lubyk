#include "rubyk.h"
#include <sstream>

class Test : public Node
{
public:
  /** Initialization code. The init code is run after the node is properly initialized. */
  bool init()
  {
    mMessage = "Hello World !";
    return true;
  }
  
  /** Example to set an attribute stored as a Value (a String here). This is also an accessor (used by sending a nil value). */
  const Value message (const Value& val)
  {
    mMessage = val;
    return mMessage;
  }
  
  /** Test output. */
  const Value out_message (const Value& val)
  {
    //if (String(val) == "is output ok?")
    //  *mOutput << "Output set" << std::endl;
    
    return gNilValue;
  }
  
  virtual const Value inspect(const Value& val) 
  { 
    // TODO: optimize (maybe).
    std::ostringstream os(std::ostringstream::out);
    os << "'" << mMessage << "' counter:" << mCounter;
    return String(os.str());
  }
  
  // [1]. Increments mCounter on bang.
  void bang(const Value& val)
  {
   // if (val.is_number())
   //   mCounter = val || mCounter;
   // else if (val.is_bang())
   //   send(++mCounter);
   // 
   // // special nil sending test
   // *mOutput << "sending nil=>";
   // send(2, gNilValue);
   // *mOutput << "<=done.\n";
  }
  
  /** [2]. Example to set an attribute stored as a native type (an int here). This is also an accessor (used by sending a nil value). */
  void counter (const Value& val)
  {
    mCounter = val || mCounter;
  }
  
  /** [3]. Displays the values sent. */
  void info(const Value& val)
  {
    //*mOutput << val << std::endl;
  }
  
  /** Another method to test the "TRY" macro. */
  const Value test_fail(const Value& p)
  {
    if (!test_try()) {
      return Error("try failed");
    }
    return gNilValue;
  }
  
  /** Class method that displays 'Hello: <parameter>'. */
  static const Value hello(Root& root, const Value& val)
  {
    return String("Hello: ").append(val.to_string());
  }
  
  
private:
  bool test_try() {
    Matrix m,n;
    m.set_sizes(5,1);
    m.fill(2.0);
//    TRY(n, copy(m, 10, 10));
    return true;
  }
  
  /* data */
  String mMessage;
  int mCounter;
};


extern "C" void init(Root& root)
{ 
  
  // Define class.
  CLASS (   Test, "Object used for testing. Does not do anything really useful.")
  
  // Define a class method.
  CLASS_METHOD( Test,  hello,  "If the input value is 0: stop. If it is greater the 0: start. Bang toggles on/off." )
  
  // Define an accessor for an attribute not used as inlet.
  METHOD( Test, message,   "Example of value storage (String)." )
  
/*  
  // Define inlets.
  // This also creates "/nodes/t/info" set/get methods and "nodes/t/inlets/info" to link, unlink, ...).
  // The get method of an inlet always returns the code defined in the inlet definition.
  //        class  inlet     accept type              info                                  return value for accessor
  INLET (   Test,  bang,     NumberValue | BangValue, "Set counter | increment and send.",  Number(mCounter))
  INLET (   Test,  counter,  NumberValue,             "Example of value storage (real_t).", Number(mCounter))
  INLET (   Test,  info,     AnyValue,                "Printout value.",                    gNilValue)
  
  // Define an outlet (also creates "/nodes/Test/outlets/result" to link, unlink, etc).
  // To "spy" on the outlet's result by hovering connection/outlet, we must register as observer on the outlet (=osc connection).
  OUTLET(   Test, counter,   NumberValue, "Increasing counter.")
  OUTLET(   Test, nil,       NilValue,    "Sends nil values.")
  
  
*/
}
