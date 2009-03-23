#include "lua_script.h"
#include "serial/serial.h"

#define DEFAULT_WINDOW_COUNT 5

class Serial : public LuaScript
{
public:

  bool init ()
  {
    return init_serial(p);
  }
  
  bool set (const Value& p)
  {
    // cannot change parameters during runtime, yet
    *mOutput << mName << ": Serial object has no parameters to change during runtime.\n";
    return set_serial(p);
  }
  
  bool set_serial(const Value& p)
  {
    return true;
  }
  
  bool init_serial (const Value& p)
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
    
    loop_me();
    return set_lua(p);
  }
  
  static void list(std::ostream * pOutput, const Value& p)
  {
    // todo
    *pOutput << "todo\n";
  }
  
  // inlet 1
  void bang(const Value val)
  { 
    int c;
    if (mLua) {
      // return value from 'update'
      call_lua("bang");
    } else {
      if (mPort.read_char(&c)) {
        send((int)c);
      }
    }
  }
  
  /** methods for lua. */
  
  /** read a character or return nil. */
  int read_char()
  {
    int c;
    if (mPort.read_char(&c)) {
      lua_pushinteger(mLua,c);
      return 1;
    } else {
      return 0;
    }
  }
  
protected:
  std::string mPortName;
  SerialPort mPort;
};

#ifdef COMPILE_SERIAL_OBJECT
// only when making 'Serial' and not a subclass
extern "C" void init()
{
  CLASS (Serial)
  OUTLET(Serial,data)
  CLASS_METHOD(Serial, list)
  METHOD_FOR_LUA(Serial, read_char)
  SUPER_METHOD(Serial, Script, load)
  SUPER_METHOD(Serial, Script, script)
}
#endif