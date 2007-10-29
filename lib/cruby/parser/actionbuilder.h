#ifndef _ACTION_BUILDER_H_
#define _ACTION_BUILDER_H_

#include <string>
#include <iostream>
#include "token.h"

typedef enum action_types_ {
	CREATE_INSTANCE,
	CREATE_LINK
} action_types_t;

class ActionBuilder
{
public:
	void setVariable  (const Token v) {
		mVariable = std::string(v.str);
	}
	void setClass  (const Token c) {
		mClass = std::string(c.str);
	}
	void setAction (action_types_t pAction) {
		mAction = pAction;
	}
	
	void createInstance(Token pVariable, Token pClass, Token pParams) {
		mAction = CREATE_INSTANCE;
		setVariable(pVariable);
		setClass(pClass);
	}
	
	void print(void) {
		switch(mAction) {
		case CREATE_INSTANCE:
			std::cout << '[' << mVariable << " = new " << mClass  << std::endl;
			break;
		case CREATE_LINK:
			std::cout << "[todo]" << std::endl;
		}
	}
	
	
private:
	action_types_t mAction;
	std::string    mVariable;
	std::string    mClass;
	std::string    mParameters;
};

#endif