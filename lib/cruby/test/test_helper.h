#ifndef _NODE_TESTER_H_
#define _NODE_TESTER_H_
#include "class.h"
#include "command.h"
#include <sstream>

class NodeTester
{
public:
  NodeTester () : mOutput(std::ostringstream::out), mInput(std::istringstream::in)  {}
  NodeTester (const char * pClassName) : mClassName(pClassName), mOutput(std::ostringstream::out), mInput(std::istringstream::in)  {}
  
protected:
  std::ostringstream mOutput;
  std::istringstream mInput;
  std::string mClassName;
  Node * mNode;
  
  void create(const char * pClass, const char * pParams)
  {
    mClassName = pClass;
    mNode = Class::create(NULL, "n", pClass, pParams, &mOutput);
  }
  
  void create(const char * pParams)
  {
    mNode = Class::create(NULL, "n", mClassName, pParams, &mOutput);
  }
  
  void assert_method_result(const char * pMethod, const char * p, const char * pOutput)
  {
    if (!mNode) {
      printf("Node not set !\n Test aborted.\n");
      TS_ABORT();
    }
    mNode->set_output(&mOutput);
    mOutput.str(std::string("")); // clear output
    mNode->execute_method(pMethod, Params(p));
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void assert_spy(const char* pSpy)
  {
    if (!mNode) {
      printf("Node not set !\n Test aborted.\n");
      TS_ABORT();
    }
    TS_ASSERT_EQUALS( std::string(mNode->get_spy()), std::string(pSpy) );
  }
  
  void assert_inspect(const char* pSpy)
  {
    if (!mNode) {
      printf("Node not set !\n Test aborted.\n");
      TS_ABORT();
    }
    TS_ASSERT_EQUALS( std::string(mNode->inspect()), std::string(pSpy) );
  }
};


class SingleParseTest
{
public:
  SingleParseTest(std::string& pResult) : mServer(), 
                mOutput(std::ostringstream::out), 
                mInput(std::istringstream::in), 
                mCmd(mInput, mOutput),
                mResult(&pResult)
  { 
    mCmd.set_server(mServer);
  }
  
  const std::string& parse(const char * pInput, bool pSilent = false)
  {
    mOutput.str(std::string("")); // clear output
    mServer.unlock();
      mCmd.set_silent(pSilent);
      mCmd.parse(pInput);
      mCmd.set_silent(false);
    mServer.lock();
    *mResult = mOutput.str();
    return *mResult;
  }
  
  const std::string& bang(const char * pInput)
  {
    mCmd.parse("\nn.bang\n");

    mServer.unlock();
      mCmd.parse("print=Print()\nn => print\n");
      mCmd.parse(pInput);
      mOutput.str(std::string("")); // clear output
      mCmd.set_silent(true);
      mCmd.parse("\nn.bang\n");
      mCmd.set_silent(false);
    mServer.lock();
    *mResult = mOutput.str();
    return *mResult;
  }
  
  const std::string& run(time_t pLength, const char * pInput)
  {
    time_t start;
    mServer.unlock();
      mCmd.parse("print=Print()\n");
    mServer.lock();
    mOutput.str(std::string("")); // clear output
    mInput.str(std::string(pInput)); // set input
    mCmd.set_silent(true);
    mServer.listen_to_command(mCmd);
    start = mServer.mCurrentTime;
    while(mServer.mCurrentTime <= start + pLength && mServer.run()) {
      mServer.run();
    }
    *mResult = mOutput.str();
    return *mResult;
  }
  
private:
  Rubyk mServer;
  Command mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  std::string *      mResult;
};

class ParseTest
{
protected:
  void assert_result(const char * pInput, const char * pOutput)
  {
    std::string result;
    SingleParseTest p(result);
    
    TS_ASSERT_EQUALS( p.parse(pInput), std::string(pOutput));
  }
  
  void assert_bang(const char * pInput, const char * pOutput)
  { 
    std::string result;
    SingleParseTest p(result);
    
    TS_ASSERT_EQUALS( p.bang(pInput), std::string(pOutput));
  }
  
  void assert_print(const char * pInput, const char * pOutput)
  { 
    std::string result;
    SingleParseTest p(result);
    TS_ASSERT_EQUALS( p.parse(pInput, true), std::string(pOutput));
  }
  
  void assert_run(time_t pLength, const char * pInput, const char * pOutput)
  {
    std::string result;
    SingleParseTest p(result);
    TS_ASSERT_EQUALS( p.run(pLength, pInput), std::string(pOutput));
  }
};

#endif