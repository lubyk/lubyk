#include "class.h"
#include "serial/serial.h"

class Serial : public Node
{
public:
  
  bool init (const Params& p)
  {
    mPort.set_output(*mOutput);
    
    if (!p.get(&mPortName, "port")) {
      *mOutput << "Port not set!\n";
      return false;
    }
    
    if (!mPort.init(mPortName, 
                    p.val("bauds",  4800), 
                    p.val("char",   8), 
                    p.val("parity", 'N'), 
                    p.val("block",   0), 
                    p.val("hard",    0),
                    p.val("soft",    0), 
                    p.val("raw",     1))) {
      *mOutput << "Could not open port.\n";
      return false;
    }
    
    // FIXME: register in loop_events
    loop_me();
    return true;
  }
  
  static void list(std::ostream * pOutput, const Params& p)
  {
    // todo
    *pOutput << "todo\n";
  }
  
  // outlet 1
  void receive(Signal& sig)
  { 
    char c;
    if (mPort.read_char(&c)) {
      sig.set((int)c);
    } else
      sig.type = NilSignal;
    
    *mOutput << sig << std::endl;
  }
  
private:
  std::string mPortName;
  SerialPort mPort;
};

extern "C" void init()
{
  CLASS (Serial)
  OUTLET(Serial,receive)
  CLASS_METHOD(Serial, list)
}