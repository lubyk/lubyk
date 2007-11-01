#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"

#undef DEBUG_PARSER

%%{
  machine command;
  write data noerror;
}%%

Command::Command(Rubyk& pServer)
{
  int cs;
  mAction = NO_ACTION;
  mServer = &pServer;
  mThread = 0;
  mQuit   = false;
  %% write init;
  mCurrentState = cs;
  mTokenIndex = 0;
}

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

  while(!mQuit) {
    mInput->getline(iss,1023); // '\n'
    parse(iss);
    parse("\n");
  }
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs = mCurrentState;        // restore machine state
  

  %%{
    action a {
      if (mTokenIndex >= MAX_TOKEN_SIZE) {
        std::cerr << "Buffer overflow !" << std::endl;
        // stop parsing
        return;
      }
#ifdef DEBUG_PARSER
      printf("_%c_",fc);
#endif
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
      if (!mQuit) {
        clear();
        prompt();
      }
    }
  
    ws     = (' ' | '\t');
  
    identifier = lower @a (alnum | '_')* @a;
  
    var    = identifier %set_var;
    
    method = identifier %set_method;
  
    klass  = upper @a (alnum | '_')* @a %set_klass;
  
    string  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* @a %set_value '"';
    float   = ('1'..'9' @a digit* @a '.' @a digit+ @a) %set_value;
    integer = ('1'..'9' @a digit* @a) %set_value;
  
    value  = (string | float | integer);
  
    key    = identifier %set_key;
  
    param  = (key ':' ws* value) @add_param;
  
    parameters = value @set_single_param | (param ws*)+;
  
    create_instance = var ws* '=' ws* klass '(' parameters? ')' ;
  
    create_link = var '.' integer %set_from ws* '=>' ws* integer '.' var;
    
    execute_method = var '.' method ( '(' parameters? ')' )?;

    execute_command = method ( '(' parameters? ')' )?;
  
    main := ((execute_command %execute_command | execute_method %execute_method | create_instance %create_instance | create_link %create_link | ws* )  '\n' )+ @prompt;
    write exec;
  }%%
    
#ifdef DEBUG_PARSER
  printf("{%i}",cs);
#endif
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
#ifdef DEBUG_PARSER
  std::cout << "[" << mTokenIndex << ":" << mToken << "]" << std::endl;
#endif
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
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  *mOutput << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
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

void Command::execute_command()
{
  Node * node = mServer->get_instance(mMethod);
  // FIXME: 
  if (node) {
    // inspect
    *mOutput << node->inspect() << std::endl;
  } else if (mMethod == "quit") {
    mServer->quit(); // not all commands should quit the server...
    mQuit = true;
    *mOutput << "Bye..." << std::endl;
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
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
