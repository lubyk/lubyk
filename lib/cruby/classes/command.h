#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include "parser/token.h"
#include "params.h"
#include "hash.h"
#include "node.h"

#define MAX_TOKEN_SIZE 2048

typedef enum action_types_ {
  NO_ACTION,
	CREATE_INSTANCE,
	CREATE_LINK
} action_types_t;

class Rubyk;

class Command
{
public:
  Command(Rubyk& pServer) : mAction(NO_ACTION), mServer(&pServer), mThread(0), mQuit(false) {}
  
  /** This method creates a new thread to listen for incomming commands. */
  void listen (std::istream& pInput, std::ostream& pOutput) ;
  
  /** Wrapper to call member method. */
  static void * call_do_listen(void * cmd) {
    ((Command*)cmd)->do_listen();
  }
  /** Stop listening for incomming commands. */
  void close ();
  
  /** Callback used by the parser to clear the current command. */
  void clear ();
  
  /** Ragel parser. */
  void parse (const char * pStr) {
    parse(std::string(pStr));
  }
  void parse (const std::string& pStr);
	
private:
  /** Code executed in a separate thread. Runs until 'mQuit' is true. */
  void do_listen();
  
  /** PARSER RELATED CALLBACKS **/
  
  /** Callback used by the parser to set a variable from the current token content. */
  void setFromToken (std::string& pElem);
  /** Callback used by the parser to set the class name. */
  void setClassFromToken  ();
  /** Callback used by the parser to set the 'class' parameter ('value' for Value, 'metro' for Metro, etc). */
  void setSingleParamFromToken ();
  /** Callback used by the parser to set a parameter. */
  void setParameter  (const std::string& pKey, const std::string& pValue);
  /** Callback used by the parser to create an instance. */
  void createInstance ();
  /** Callback used by the parser to create a link. If the link cannot be created right now because all variables aren't set yet, the link will be kept in Rubyk's link buffer until all variables are found. */
  void createLink ();
  /** Callback used by the parser to print the current command. */
  void printCommand ();
  
  /** Token for the parser. */
  char mToken[MAX_TOKEN_SIZE + 1];
  unsigned int   mTokenIndex;
  
	action_types_t mAction;
  
  /** Command parts. */
	std::string    mVariable, mClass, mSingleParam, mKey, mValue, mFrom, mTo;
	Params         mParameters;
  int            mFromPort, mToPort;
  Rubyk *        mServer;
  
  /** IO management. */
  pthread_t mThread;
  std::istream * mInput;
  std::ostream * mOutput;
  /* Listen elements. */
  bool mQuit;
};

#endif