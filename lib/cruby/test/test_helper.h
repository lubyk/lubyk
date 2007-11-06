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
    mNode = Class::create(NULL, pClass, pParams, &mOutput);
  }
  
  void create(const char * pParams)
  {
    mNode = Class::create(NULL, mClassName, pParams, &mOutput);
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


class ParseTest
{
public:
  ParseTest() : mServer(), mCmd(&mServer), mOutput(std::ostringstream::out), mInput(std::istringstream::in)  
    { mCmd.set_input(mInput); mCmd.set_output(mOutput); }

private:
  Rubyk mServer;
  Command mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
protected:
  void assert_result(const char * pInput, const char * pOutput)
  {
    mOutput.str(std::string("")); // clear output
    mCmd.parse(pInput);
    mServer.run(); // run one loop to execute commands
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void assert_prints(const char * pInput, const char * pOutput)
  {  
    mCmd.parse("print=Print()\n");
    mOutput.str(std::string("")); // clear output
    mCmd.set_silent(true);
    mCmd.parse(pInput);
    //mServer.run(); // loop once
    mCmd.set_silent(false);
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void assert_run(time_t pLength, const char * pInput, const char * pOutput)
  {
    time_t start;
    mCmd.parse("print=Print()\n");
    mOutput.str(std::string("")); // clear output
    mOutput.str(std::string("")); // clear output
    mCmd.set_silent(true);
    mCmd.parse(pInput);
    
    start = mServer.mCurrentTime;
    while(mServer.mCurrentTime <= start + pLength && mServer.run()) {
      ;
    }
    mCmd.set_silent(false);
    
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
};

#endif