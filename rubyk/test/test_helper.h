#ifndef _NODE_TESTER_H_
#define _NODE_TESTER_H_
#include "class.h"
#include "command.h"
#include <sstream>


#define assert_print(x,y) _assert_print(__FILE__,__LINE__,x,y)

#define ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m) CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (m) )
#define ___RK_ASSERT_EQUALS(f,l,cmd,x,y,m) { _TS_TRY { ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m); } __TS_CATCH(f,l) }
#define _RK_ASSERT_EQUALS(f,l,cmd,x,y) ___RK_ASSERT_EQUALS(f,l,cmd,x,y,0)

/** Usage:
* setup: creates an initial state with a Print node called 'p'.
* parse: parse commands.

*/
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

class ParseTest : public CxxTest::TestSuite
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
  void setUp()
  {
    mServer = new Rubyk;
    mCmd = new Command(mInput, mOutput);
    mCmd->set_server(*mServer);
    mOutput.str(std::string("")); // clear output
    parse("p=Print()\n");
  }
  
  // cleanup
  void tearDown()
  {
    delete mServer;
    delete mCmd;
  }
  
protected:
  Rubyk * mServer;
  Command * mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
protected:
  void parse(const char* pCommands)
  {
    mServer->unlock();
      mCmd->parse(pCommands);
      mOutput.str(std::string("")); // clear output
    mServer->lock();
  }
  
  void assert_result(const char * pInput, const char * pOutput)
  {
    mOutput.str(std::string("")); // clear output
    mServer->unlock();
      mCmd->parse(pInput);
    mServer->lock();
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
  
  void _assert_print(const char * file, int lineno, const char * pCommands, const char * pResult)
  { 
    mServer->unlock();
      mOutput.str(std::string("")); // clear output
      mCmd->set_silent(true);
      mCmd->parse(pCommands);
      mCmd->set_silent(false);
    mServer->lock();
    
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pCommands)), mOutput.str(), std::string(pResult));
  }
  
  void assert_run(time_t pLength, const char * pResult)
  {
    time_t start;
    mOutput.str(std::string("")); // clear output
    mCmd->set_silent(true);
      start = mServer->mCurrentTime;
      while(mServer->mCurrentTime <= start + pLength && mServer->run())
        ;
    mCmd->set_silent(false);
    
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pResult));
  }
};
#endif