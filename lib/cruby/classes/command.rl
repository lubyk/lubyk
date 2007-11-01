#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"

%%{
  machine command;
  write data noerror;
}%%

Command::~Command()
{
  if (mThread) close();
}

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
  char iss[1024];
  *mOutput << "Welcome to rubyk !\n\n";
  clear();
  prompt();

  while(!mQuit && !mInput->eof()) {
    mInput->getline(iss,1023); // '\n'
    parse(iss);
  }
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs = mCurrentState;        // restore machine state
  
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
      set_from_token(mVariable);
    }
    
    action set_method {
      set_from_token(mMethod);
    }
    
    action set_key {
      set_from_token(mKey);
    }
  
    action set_klass {
      set_class_from_token();
    }
  
    action set_value {
      set_from_token(mValue);
    }
    
    action set_from {
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
  
    action create_instance {
      create_instance();
    }
    
    action set_single_param {
      set_single_param_from_token();
    }
    
    action add_param {
      set_parameter(mKey, mValue);
    }
    
    action create_link {
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
    
    action execute_method {
      execute_method();
    }
    
    action execute_command {
      execute_command();
    }
    
    action debug {
      printf("[%i]", cs);
    }
    
    action prompt {
      clear();
      prompt();
    }
  
    ws     = (' ' | '\n' | '\t');
  
    identifier = 'a'..'z' @a (digit | alpha | '_')* @a;
  
    var    = identifier %set_var;
    
    method = identifier %set_method;
  
    klass  = 'A'..'Z' @a (digit | alpha | '_')* @a %set_klass;
  
    string  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* @a %set_value '"';
    float   = ('1'..'9' @a digit* @a '.' @a digit+ @a) %set_value;
    integer = ('1'..'9' @a digit* @a) %set_value;
  
    value  = (string | float | integer);
  
    key    = identifier %set_key;
  
    param  = (key ':' ws* value) %add_param;
  
    parameters = value @set_single_param | (param ws*)+;
  
    create_instance = var ws* @debug '=' @debug ws* klass '(' parameters? ')' @create_instance;
  
    create_link = var '.' integer @set_from ws* '=>' ws* integer '.' var @create_link;
    
    execute_method = var '.' method ( '(' parameters? ')' ) @execute_method;
  
    main := (execute_method | create_instance | create_link | ws+ )+ @prompt;
    
    write init;
    write exec;
  }%%
  mCurrentState = cs;
}


void Command::close() {
  mQuit = true;
  // FIXME: how to force out of input (>>) ?
  if (mThread)
    pthread_join( mThread, NULL); // wait for listen to finish
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
  std::cout << "[" << mToken << "]" << std::endl;
  pElem = mToken;
  mTokenIndex = 0;
}


void Command::set_class_from_token  () 
{
  set_from_token(mClass);
  if (mSingleParam != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
    mSingleParam = "";
  }
}

void Command::set_single_param_from_token () 
{
  set_from_token(mSingleParam);
  if (mClass != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
  }
}

void Command::set_parameter  (const std::string& pKey, const std::string& pValue) 
{
	mParameters.set(pKey,pValue);
}

void Command::create_instance()
{
  Node * node = mServer->create_instance(mVariable, mClass, mParameters);
  if (node) {
    *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
}


void Command::create_link()
{
  *mOutput << "LINK (" << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << ")" << std::endl;
}

void Command::execute_method()
{
  Node * node = mServer->get_instance(mVariable);
  if (node) {
    node->execute_method(mMethod, mParameters);
    *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
}

void Command::print(void)
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
