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
  Node * mNode;
  std::string mClassName;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
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

class ParseTest
{
public:
  ParseTest() : mOutput(std::ostringstream::out), 
                mInput(std::istringstream::in)
                
  { 
    mServer = new Rubyk;
    mCmd = new Command(mInput, mOutput);
    mCmd->set_server(*mServer);
    // cannot set class lib path here (not yet initialized).
  }
  
  // start a new server
  void clean_start()
  {
    delete mServer;
    delete mCmd;
    
    mServer = new Rubyk;
    mCmd = new Command(mInput, mOutput);
    mCmd->set_server(*mServer);
    mOutput.str(std::string("")); // clear output
  }
  
protected:
  Rubyk * mServer;
  Command * mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
protected:
  
  void setup_with_print(const char* pInput)
  {
    clean_start();
    mServer->unlock();
      mCmd->parse("p=Print()\nn=>p\n");
      mCmd->parse(pInput);
      mOutput.str(std::string("")); // clear output
    mServer->lock();
  }
  
  void clean_assert_result(const char * pInput, const char * pOutput)
  {
    clean_start();
    assert_result(pInput, pOutput);
  }
  void assert_result(const char * pInput, const char * pOutput)
  {
    mOutput.str(std::string("")); // clear output
    mServer->unlock();
      mCmd->parse(pInput);
    mServer->lock();
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void clean_assert_bang(const char * pInput, const char * pOutput)
  {
    setup_with_print(pInput);
    assert_bang("\n", pOutput);
  }
  
  void assert_bang(const char * pInput, const char * pOutput)
  { 
    mServer->unlock();
      mOutput.str(std::string("")); // clear output
      mCmd->set_silent(true);
      mCmd->parse(pInput);
      mCmd->parse("\nn.bang\n");
      mCmd->set_silent(false);
    mServer->lock();
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void clean_assert_print(const char * pInput, const char * pOutput)
  {
    clean_start();
    assert_print(pInput, pOutput);
    
  }
  void assert_print(const char * pInput, const char * pOutput)
  { 
    mServer->unlock();
      mOutput.str(std::string("")); // clear output
      mCmd->set_silent(true);
      mCmd->parse(pInput);
      mCmd->set_silent(false);
    mServer->lock();
    //mServer->run(); // loop once
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void clean_assert_run(time_t pLength, const char * pInput, const char * pOutput)
  {
    clean_start();
    assert_run(pLength, pInput, pOutput);
  }
  void assert_run(time_t pLength, const char * pInput, const char * pOutput)
  {
    time_t start;
    mServer->unlock();
      mCmd->parse("print=Print()\n");
    mServer->lock();
    mOutput.str(std::string("")); // clear output
    mInput.str(std::string(pInput)); // set input
    mCmd->set_silent(true);
    mServer->listen_to_command(*mCmd);
    start = mServer->mCurrentTime;
    while(mServer->mCurrentTime <= start + pLength && mServer->run()) {
      ;
    }
    mCmd->set_silent(false);
    
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
};

#endif