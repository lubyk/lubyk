#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"

%%{
  machine command;
  write data noerror;
}%%

void Command::listen (std::istream& pInput, std::ostream& pOutput)
{
  int ret;
  if (mThread) {
    close();
    mQuit = false;
  }
  mOutput = &pOutput;
  mInput  = &pInput;
  ret = pthread_create( &mThread, NULL, &Command::call_do_listen, this);
  // FIXME: check for error from 'ret'
}

void Command::do_listen()
{
  std::string iss;
  *mOutput << "Welcome to rubyk !\n\n";
  
  while(!mQuit && !mInput->eof()) {
    *mInput >> iss;
    parse(iss);
  }
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs;        // machine state
  
  mTokenIndex = 0;

  %%{
    action a {
      if (mTokenIndex >= MAX_TOKEN_SIZE) {
        std::cerr << "Buffer overflow !" << std::endl;
        // stop parsing
        return;
      }
      
      mToken[mTokenIndex] = fc; /* append */
      mTokenIndex++;     
    }
    action set_var {
      setFromToken(mVariable);
    }
    
    action set_key {
      setFromToken(mKey);
    }
  
    action set_klass {
      setClassFromToken();
    }
  
    action space {
      printf(" ");
    }
  
    action ret {
      printf("\n");
    }
  
    action set_value {
      setFromToken(mValue);
    }
    
    action set_from {
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
  
    action create_instance {
      //std::cout << "NEW (" << mVariable << " = " << mClass << "(" << mParameters << ")" << ")" << std::endl;
      createInstance();
    }
    
    action set_single_param {
      setSingleParamFromToken();
    }
    
    action add_param {
      setParameter(mKey, mValue);
    }
    
    action create_link {
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      *mOutput << "LINK (" << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << ")" << std::endl;
    }
  
    ws     = (' ' | '\n' | '\t')+;
  
    identifier = 'a'..'z' @a (digit | alpha | '_')* @a;
  
    var    = identifier %set_var;
  
    klass  = 'A'..'Z' @a (digit | alpha | '_')* @a %set_klass;
  
    string  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* @a %set_value '"';
    float   = ('1'..'9' @a digit* @a '.' @a digit+ @a) %set_value;
    integer = ('1'..'9' @a digit* @a) %set_value;
  
    value  = (string | float | integer);
  
    key    = identifier %set_key;
  
    param  = (key ':' ws* value) %add_param;
  
    parameters = value @set_single_param | (param ws*)+;
  
    create_instance = var ws* '=' ws* klass '(' parameters? ')' @create_instance;
  
    create_link = var '.' integer @set_from ws* '=>' ws* integer '.' var @create_link;
  
    main := ((create_instance | create_link) ws*)+  ;
    
    write init;
    write exec;
  }%%
}


void Command::close() {
  mQuit = true;
  // FIXME: how to force out of input (>>) ?
  if (mThread)
    pthread_join( mThread, NULL); // wait for listen to finish
}

void Command::setFromToken (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
  //std::cout << "[" << mToken << "]" << std::endl;
  pElem = mToken;
  mTokenIndex = 0;
}


void Command::setClassFromToken  () 
{
  setFromToken(mClass);
  if (mSingleParam != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
    mSingleParam = "";
  }
}

void Command::setSingleParamFromToken () 
{
  setFromToken(mSingleParam);
  if (mClass != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
  }
}

void Command::setParameter  (const std::string& pKey, const std::string& pValue) 
{
	mParameters.set(pKey,pValue);
}

void Command::createInstance()
{
  Node * node = mServer->create_instance(mVariable, mClass, mParameters);
  if (node) {
    *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
  clear();
}

void Command::printCommand(void)
{
	switch(mAction) {
	case CREATE_INSTANCE:
	  *mOutput << mVariable << " = " << mClass << "(" << mParameters << ")" << std::endl;
		break;
	case CREATE_LINK:
		*mOutput << "[todo]" << std::endl;
    break;
	}
}

void Command::clear() 
{
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mSingleParam = "";
  mParameters.clear();
}
