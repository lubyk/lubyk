#include "command.h"

void Command::listen (std::istream& pInput, std::ostream& pOutput)
{
  pOutput << "Start" << std::endl;
}

void Command::setVariable  (const Token& v) 
{
	mVariable = std::string(v.str);
}

void Command::setClass  (const Token& c) 
{
	mClass = std::string(c.str);
  if (mClassParam != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mClassParam);
    mClassParam = "";
  }
}

void Command::setClassParameter (const Token& c) 
{
  if (mClass != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, c.str);
  } else {
		mClassParam = std::string(c.str);
  }
}

void Command::setParameter  (const Token& pKey, const Token& pValue) 
{
	mParameters.set(pKey.str,pValue.str);
}

void Command::createInstance()
{
  Node * node = Node::create(mClass, mParameters);
  Node ** previous;
	mAction = CREATE_INSTANCE;
  if (node) {
    previous = mInstances.get(mVariable);
    if (previous) {
      // kill the node pointed by variable name
      delete *previous;
    }
    mInstances.set(mVariable, node);
    std::cout << node->inspect() << std::endl;
  } else {
    std::cout << "Error" << std::endl;      
  }
  printCommand();
  clear();
}

void Command::printCommand(void)
{
	switch(mAction) {
	case CREATE_INSTANCE:
	  std::cout << mVariable << " = " << mClass << "(" << mParameters << ")" << std::endl;
		break;
	case CREATE_LINK:
		std::cout << "[todo]" << std::endl;
    break;
	}
}

void Command::clear() 
{
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mClassParam = "";
  mParameters.clear();
}
