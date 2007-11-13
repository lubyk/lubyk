#include "lua_script.h"
#include "serial/serial.h"

#define DEFAULT_WINDOW_COUNT 5

class Serial : public LuaScript
{
public:
  
  ~Serial ()
  {
    if (mBuffer) free(mBuffer);
  }
  
  bool init (const Params& p)
  {
    return init_serial(p);
  }
  
  bool init_serial (const Params& p)
  {
    
    if (p.get(&mWindowSize, "window") && mWindowSize) {
      // use buffer
      size_t sz = mWindowSize * DEFAULT_WINDOW_COUNT; // write twice once every 10 windows
      mBuffer = (double*)malloc(sz * sizeof(double));
      if (!mBuffer) {
        *mOutput << "Could not allocate '" << sz << "' doubles.\n";
      } else {
        for(int i= 0;i<mWindowSize;i++)
          mBuffer[i] = 0.0;
        mNextWrite = mWindowSize - 1;
        mReadPosition = 0;
        mReadOffset   = 0;
      }
    }
    
#ifdef _TEST_OBJECTS_
    if (p.get(&mTestData, "test")) {
      return init_lua(p);
      mTestDataIndex = 0;
    } else {
      mTestDataIndex = -1;
    }
#endif

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
    return init_lua(p);
  }
  
  static void list(std::ostream * pOutput, const Params& p)
  {
    // todo
    *pOutput << "todo\n";
  }
  
  // inlet 1
  void action(const Signal& sig)
  {
    // do nothing
  }
  
  // outlet 1
  void receive(Signal& sig)
  { 
    int c;
    if (mLua) {
      if (mBuffer) {
        call_lua("update", sig);
        if (sig.type == BangSignal) {
          // return pointer to array of size mWindowSize (first element = oldest)
          sig.type = FloatArraySignal;
          sig.floats.size  = mWindowSize;
          sig.floats.value = mBuffer + mReadPosition;
        }
      } else {
        // return value from 'update'
        call_lua("update", sig);
      }
    } else {
      if (mPort.read_char(&c)) {
        sig.set((int)c);
      } else
        sig.set_nil();
    }
  }
  
  /** methods for lua. */
  
  /** read a character or return nil. */
  int read_char()
  {
    int c;
#ifdef _TEST_OBJECTS_
    if (mTestDataIndex != -1) {
      if (mTestDataIndex >= mTestData.size())
        return 0; //eof
      else {
        lua_pushinteger(mLua, mTestData.c_str()[mTestDataIndex]);
        mTestDataIndex++;
        return 1;
      }
    } else if (mPort.read_char(&c)) {
#else
    if (mPort.read_char(&c)) {
#endif
      lua_pushinteger(mLua,c);
      return 1;
    } else {
      return 0;
    }
  }
  
  /** Called by lua to fill the buffer (after filtering). */
  int write()
  {
    if (lua_istable(mLua, 1)) {
      int i = 1;
      while(true) {
        lua_pushinteger(mLua, i);
        lua_gettable(mLua, 1);
        if(!lua_isnumber(mLua, -1))
          break;
          float f = lua_tonumber(mLua, -1);
          write_buf(f);
          lua_pop(mLua,1);
          i++;
      }    
      lua_pop(mLua,1);
    } else {
      float f = lua_tonumber(mLua, 1);
      write_buf(f);
    }
    return 0;
  }
  
  /** Set by lua. A read offset of 0 means that we read all the data to the latest. An offset of 5 means we do not send the last 5 floats. The offset cannot be greater then the window size. */
  int set_read_offset()
  {
    mReadOffset = lua_tointeger(mLua, 1) % mWindowSize;
  }
  
  inline void write_buf(float f)
  {
    if (!mBuffer) {
      *mOutput << "Serial error: trying to write to buffer without window.\n";
      return;
    }
    mBuffer[mNextWrite] = f;
    
    if (mNextWrite >= (mWindowSize * (DEFAULT_WINDOW_COUNT - 1))) {
      // copy value to loop head 
      mBuffer[mNextWrite - (mWindowSize * (DEFAULT_WINDOW_COUNT - 1))] = f;
    }
    
    mReadPosition = mNextWrite - mWindowSize + 1 - mReadOffset;
    
    if (mReadPosition < 0)
      mReadPosition = (mWindowSize * (DEFAULT_WINDOW_COUNT - 1)) + mReadPosition;
    
    mNextWrite++;
    
    if (mNextWrite >= (mWindowSize * DEFAULT_WINDOW_COUNT)) {
      mNextWrite = mWindowSize;
    }
    
  }
  
protected:
  double * mBuffer;
  std::string mPortName;
#ifdef _TEST_OBJECTS_
  std::string mTestData; /**< Used to fake the serial port for testing. */
  int         mTestDataIndex;
#endif
  SerialPort mPort;
  int      mWindowSize;
  int      mNextWrite;
  int      mReadPosition;
  int      mReadOffset;
};

#ifdef COMPILE_SERIAL_OBJECT
// only when making 'Serial' and not a subclass
extern "C" void init()
{
  CLASS (Serial)
  INLET(Serial,action)
  OUTLET(Serial,receive)
  CLASS_METHOD(Serial, list)
  METHOD_FOR_LUA(Serial, read_char)
  METHOD_FOR_LUA(Serial, write)
  METHOD_FOR_LUA(Serial, set_read_offset)
  SUPER_METHOD(Serial, Script, set)
  SUPER_METHOD(Serial, Script, load)
  SUPER_METHOD(Serial, Script, script)
}
#endif