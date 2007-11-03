#ifndef _NODE_TESTER_H_
#define _NODE_TESTER_H_

class NodeTester
{
public:
  NodeTester () : mOutput(std::ostringstream::out), mInput(std::istringstream::in)  {}

private:
  std::ostringstream mOutput;
  std::istringstream mInput;
  
protected:  
  Node * mNode;
  
  void assert_method_result(const char * pMethod, const char * p, const char * pOutput)
  {
    if (!mNode) {
      printf("Node not set !\n Test aborted.\n");
      TS_ABORT();
    }
    mOutput.str(std::string("")); // clear output
    mNode->execute_method(pMethod, Params(p), &mOutput);
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

#endif