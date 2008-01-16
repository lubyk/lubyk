#include "command.h"
#include "rubyk.h"
#include "class.h"
#include "mutex.h"

//#define DEBUG_PARSER

%%{
  machine command;
  write data noerror;
}%%

void Command::initialize()
{
  int cs;
  
  mAction = NO_ACTION;
  mServer = NULL;
  mQuit   = false;
  mTokenIndex = 0;
  mSilent = false;
  mThread = NULL;
  
  clear();
  
  %% write init;
  mCurrentState = cs;
}

int Command::do_listen()
{
  char buffer[1024];
  char * line = buffer;
  
  // set thread priority to normal
  mServer->normal_priority();
  
  if (!mSilent)
    *mOutput << "Welcome to rubyk !\n\n";
  clear();

  while(!mQuit && getline(&line,1023)) {
    parse(line);
    parse("\n");
    if (!mQuit)
      saveline(line);
    freeline(line);
  }
  return 0; // thread return value
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
    
    action set_var { set_from_token(mVariable);}
    
    action set_method { set_from_token(mMethod);}
    
    action set_key { set_from_token(mKey);}
  
    action set_class { set_from_token(mClass);}
  
    action set_value { set_from_token(mValue);}
    
    action set_from { mFrom = mVariable; }
    
    action set_from_port {
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
    }
    
    action set_to_port {
      set_from_token(mValue);
      mToPort = atoi(mValue.c_str());
    }
    
    action add_param_value { 
      add_value_from_token(); 
    }
    
    action add_param { set_parameter(mKey, mValue); }
    
    action create_link {
      mTo   = mVariable;
      create_link();
    }
    
    action remove_link {
      mTo   = mVariable;
      remove_link();
    }

    action create_instance { create_instance(); }
    
    action execute_method { execute_method(); }
    
    action execute_class_method { execute_class_method(); }
    
    action execute_command { execute_command(); }
    
    action debug { printf("[%c]", p[0]); }
    
    action prompt {
      if (!mQuit) {
        clear();
      }
    }
    action error {
      fhold; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      fgoto eat_line; // eat the rest of the line and continue parsing
    }
  
    eat_line := [^\n]* '\n' @{ fgoto main; };
    
    comment = '#' [^\n]* ;
    
    ws     = (' ' | '\t');
  
    identifier = (lower (alnum | '_')*) $a;
  
    var    = identifier %set_var;
    
    method = identifier %set_method;
  
    class  = (upper (alnum | '_')*) $a %set_class;
  
    string  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* $a '"';
    float   = ([\-+]? $a ('0'..'9' digit* '.' digit+) $a );
    integer = ([\-+]? $a ('0'..'9' digit*) $a );
  
    value  = (string | float | integer ) %set_value ;
  
    key    = identifier %set_key;
  
    param  = (key ':' ws* value) %add_param;
  
    parameters = value %add_param_value (ws* ',' ws* value %add_param_value)* | (param ws*)+;
  
    create_instance = var ws* '=' ws* class '(' parameters? ')' ;
  
    from_link = var %set_from ('.' integer %set_from_port)?;
    to_link   = (integer %set_to_port '.')? var;
    create_link = from_link ws* '=>' ws* to_link;
    
    remove_link = from_link ws* '//' ws* to_link;
    
    execute_method       = var   '.' method ( '(' parameters? ')' )? ;
    
    execute_class_method = class '.' method ( '(' parameters? ')' )? ;

    execute_command = method ( '(' parameters? ')' )?;
  
    main := ((execute_command %execute_command # cannot put comments here :-(
            | execute_method  %execute_method (ws comment)?
            | execute_class_method  %execute_class_method (ws comment)?
            | create_instance %create_instance (ws comment)?
            | create_link %create_link (ws comment)?
            | remove_link %remove_link (ws comment)?
            | comment
            | ws* ) '\n' )+ @prompt $err(error);
    write exec;
    # write eof; (add eof actions to all above before removing comment)
  }%%
//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close()
{
  mQuit = true;
  if (mThread) {
    pthread_kill(mThread, SIGINT);
  }
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
#ifdef DEBUG_PARSER
  if (&pElem == &mValue) std::cout <<    "[val " << mToken << "]" << std::endl;
  if (&pElem == &mVariable) std::cout << "[var " << mToken << "]" << std::endl;
  if (&pElem == &mClass) std::cout <<    "[cla " << mToken << "]" << std::endl;
#endif
  pElem = mToken;
  mTokenIndex = 0;
}

void Command::add_value_from_token () 
{
  mParameters.add(mValue); // add to 'list' value
}

void Command::set_parameter  (const std::string& pKey, const std::string& pValue) 
{
  mParameters.set(pKey,pValue);
}

// FIXME: create_instance should run in server space with concurrency locks.
void Command::create_instance()
{
  mServer->lock();
  Node * node = mServer->create_instance(mVariable, mClass, mParameters, mOutput);
#ifdef DEBUG_PARSER
  std::cout << "NEW "<< mVariable << " = " << mClass << "(" << mParameters << ")";
#endif
  if (node) {
    if (!mSilent)
      *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
  mServer->unlock();
}


void Command::create_link()
{ 
  mServer->lock();
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

void Command::remove_link()
{ 
  mServer->lock();
  mServer->remove_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "UNLINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mVariable)) {
    node->execute_method(mMethod, mParameters);
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_class_method()
{
  Class * klass;
  mServer->lock();
  if (Class::get(&klass, mClass)) {
    klass->execute_method(mMethod, mParameters, mOutput);
  } else {
    *mOutput << "Unknown class '" << mClass << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_command()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
    
  } else if (mMethod == "quit" || mMethod == "q") {
    mServer->quit();
    mQuit = true;
  } else if (mMethod == "set_lib_path") {
    std::string path;
    if (!mParameters.get(&path)) {
      *mOutput << "Could not set library path (no parameter).\n";
      return;
    }
    Class::set_lib_path(path);
    *mOutput << "Library path set to '" << path << "'.\n";
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::clear() 
{
  mTokenIndex = 0;
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mParameters.clear();
  mFromPort = 1;
  mToPort   = 1;
}
