#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <iostream>
#include <cstdio>
#include "token.h"
#include "params.h"
#include "hash.h"
#include "node.h"
//#include <algorithm> // to_lower

typedef enum action_types_ {
  NO_ACTION,
	CREATE_INSTANCE,
	CREATE_LINK
} action_types_t;

class Rubyk;

class Command
{
public:
  Command(/*Rubyk * pServer*/) : mAction(NO_ACTION), mInstances(200),/* mServer(pServer),*/ mQuit(false) {}
  
  /** This method creates a new thread to listen for incomming commands. */
  void listen (std::istream& pInput, std::ostream& pOutput) ;
  
  /** Stop listening for incomming commands. */
  void close () {
    mQuit = true;
  }
  
  /** PARSER RELATED CALLBACKS **/
  
  /** Callback used by the parser to set the variable name. */
  void setVariable  (const Token& v);
  /** Callback used by the parser to set the class name. */
  void setClass  (const Token& c);
  /** Callback used by the parser to set the 'class' parameter ('value' for Value, 'metro' for Metro, etc). */
  void setClassParameter (const Token& c);
  /** Callback used by the parser to set a parameter. */
  void setParameter  (const Token& pKey, const Token& pValue);
  /** Callback used by the parser to create an instance. */
  void createInstance ();
  /** Callback used by the parser to print the current command. */
  void printCommand ();
  /** Callback used by the parser to clear the current command. */
  void clear ();
	
private:
  /** Command creation elements. */
	action_types_t mAction;
	std::string    mVariable;
	std::string    mClass;
  std::string    mClassParam;
	Params         mParameters;
  Rubyk *        mServer;
  Hash<std::string, Node *> mInstances;
  
  /* Listen elements. */
  bool mQuit;
};

#endif