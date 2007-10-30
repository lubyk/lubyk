#ifndef _ACTION_BUILDER_H_
#define _ACTION_BUILDER_H_

#include <string>
#include <iostream>
#include <cstdio>
#include "token.h"
#include "params.h"
//#include <algorithm> // to_lower

typedef enum action_types_ {
  NO_ACTION,
	CREATE_INSTANCE,
	CREATE_LINK
} action_types_t;

class Command
{
public:
  Command() : mAction(NO_ACTION) {}
  
	void setVariable  (const Token& v) {
		mVariable = std::string(v.str);
	}
	
	void setClass  (const Token& c) {
  	mClass = std::string(c.str);
	  if (mClassParam != "") {
      std::string key = mClass;
      std::transform(key.begin(), key.end(), key.begin(), tolower);
      mParameters.set(key, mClassParam);
      mClassParam = "";
	  }
	}
	
	void setClassParameter (const Token& c) {
	  if (mClass != "") {
      std::string key = mClass;
      std::transform(key.begin(), key.end(), key.begin(), tolower);
      mParameters.set(key, c.str);
	  } else {
  		mClassParam = std::string(c.str);
	  }
	}
	
	void setParameter  (const Token& pKey, const Token& pValue) {
		mParameters.set(pKey.str,pValue.str);
	}
	
	void setAction (action_types_t pAction) {
		mAction = pAction;
	}
	
	void createInstance() {
		mAction = CREATE_INSTANCE;
    printCommand();
    clear();
	}
	
	void printCommand(void) {
		switch(mAction) {
		case CREATE_INSTANCE:
		  std::cout << mVariable << " = " << mClass << "(" << mParameters << ")" << std::endl;
			break;
		case CREATE_LINK:
			std::cout << "[todo]" << std::endl;
      break;
		}
	}
	
	void clear() {
    mAction     = NO_ACTION;
    mVariable   = "";
    mClass      = "";
    mClassParam = "";
    mParameters.clear();
	}
	
private:
	action_types_t mAction;
	std::string    mVariable;
	std::string    mClass;
  std::string    mClassParam;
	Params         mParameters;
};

#endif