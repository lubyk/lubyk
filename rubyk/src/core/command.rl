#include "command.h"
#include "rubyk.h"

//#define DEBUG_PARSER

%%{
  machine command;
  write data noerror;
}%%

/*
  typedef enum command_actions_ {
    CmdNoAction,
  	CmdCreateNode,
  	CmdCreateLink,
  	CmdOpenGroup,
  } command_action_t;
*/

void Command::initialize()
{
  int cs;
  
  mAction = CmdNoAction;
  mPlanet     = NULL;
  mQuit       = false;
  mTokenIndex = 0;
  mSilent     = false;
  mThread     = NULL;
  
  clear();
  
  %% write init;
  mCurrentState = cs;
}

int Command::do_listen()
{
  char buffer[1024];
  char * line = buffer;
  
  // set thread priority to normal
  mPlanet->normal_priority();
  
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
  const char *eof = NULL;  // FIXME: this should be set to 'pe' on the last pStr block...
  int cs = mCurrentState;        // restore machine state
  
#ifdef DEBUG_PARSER
  printf("parse:\"%s\"\n",pStr.c_str());
#endif
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
    
    action params {
      // FIXME: this is a temporary hack until we sub parse with Value...
      mParamString += fc;
    }
    
    action set_var    { set_from_token(mVar);}
    
    action set_method { set_from_token(mMethod);}
  
    action set_class  { set_from_token(mClass);}
  
    action set_value  { set_from_token(mValue);}
    
    action set_from      { mFrom     = mVar; }
    
    action set_from_port { mFromPort = mVar; }
    
    action set_to_port   { mToPort   = mVar; }
    
    action add_param_value { 
      add_value_from_token(); 
    }
    
    action create_link {
      mTo   = mVar;
      create_link();
    }
    
    action remove_link {
      mTo   = mVar;
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
  
    key    = identifier;
  
    param  = (key ':' ws* value);
  
    # FIXME: replace this by a string and let Value parse it into a Hash, or better, move control to Value...
    # but before fixing this, we need Value to accept mutliline content (one block after the other...): this is not easy.
    parameters = (value (ws* ',' ws* value)* | (param ws*)+) $params;
  
    create_instance = var ws* '=' ws* class '(' parameters? ')' ;
  
    from_link = var %set_from ('.' var %set_from_port)?;
    to_link   = (var %set_to_port '.')? var;
    create_link = from_link ws* '=>' ws* to_link;
    
    remove_link = from_link ws* '//' ws* to_link;
    
    execute_method       = var   '.' method ( '(' parameters? ')' )? ;
    
    execute_class_method = class '.' method ( '(' parameters? ')' )? ;

    execute_command = method ( '(' parameters? ')' )?;
  
    main := ((execute_command %execute_command # cannot put comments here :-(
            | execute_method  %execute_method (ws* comment)?
            | execute_class_method  %execute_class_method (ws* comment)?
            | create_instance %create_instance (ws* comment)?
            | create_link %create_link (ws* comment)?
            | remove_link %remove_link (ws* comment)?
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

const Value Command::get_params ()
{
  Value params(mParamString);
  mParamString = "";
  return params;
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
#ifdef DEBUG_PARSER
  if (&pElem == &mValue) std::cout << "[val " << mToken << "]" << std::endl;
  if (&pElem == &mVar)   std::cout << "[var " << mToken << "]" << std::endl;
  if (&pElem == &mClass) std::cout << "[cla " << mToken << "]" << std::endl;
#endif
  pElem = mToken;
  mTokenIndex = 0;
}

// FIXME: create_instance should run in server space with concurrency locks.
void Command::create_instance()
{
  Value params = get_params();
  
  mPlanet->lock();
    // FIXME: Group scope
    // FIXME: should be new_object(mVar, mClass, Value(mParams))
    Value res = mPlanet->new_object(mVar, mClass, params);
  mPlanet->unlock();
  
  if (res.is_string()) {
    std::string url = String(res).string();
    res = mPlanet->call(url.append("/#inspect"));
  }
  
  
#ifdef DEBUG_PARSER
  std::cout << "NEW "<< mVar << " = " << mClass << "(" << params << ")";
#endif

  if (!mSilent)
    *mOutput << res << std::endl;
}


void Command::create_link()
{ 
  mPlanet->lock();
    // FIXME: Group scope
    mPlanet->create_link(std::string("/").append(mFrom), mFromPort, mToPort, std::string("/").append(mTo));
  mPlanet->unlock();

#ifdef DEBUG_PARSER
  std::cout << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

void Command::remove_link()
{ 
  mPlanet->lock();
    // FIXME: Group scope
    mPlanet->remove_link(std::string("/").append(mFrom), mFromPort, mToPort, std::string("/").append(mTo));
  mPlanet->unlock();

#ifdef DEBUG_PARSER
  std::cout << "UNLINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Value res;
  Value params = get_params();
  
  mPlanet->lock();
    // FIXME: Group scope
    if (mMethod == "set") {
      oscit::Object * obj = mPlanet->find(std::string("/").append(mVar));
      if (obj) obj->set(params);
    } else {
      if (mMethod == "b") mMethod = "bang";
      oscit::Object * meth = mPlanet->find(std::string("/").append(mVar).append("/").append(mMethod));
      if (meth) {
        res = meth->trigger(params);
      } else {
        if (params.is_nil()) params = gBangValue;
        res = mPlanet->call(std::string("/").append(mVar).append("/in/").append(mMethod), params);
      }
    }
  mPlanet->unlock();
  if (!res.is_nil()) *mOutput << res << std::endl;
}

void Command::execute_class_method()
{
  Value res;
  Value params = get_params();
  
  mPlanet->lock();
    res = mPlanet->call(std::string(CLASS_ROOT).append("/").append(mClass).append("/").append(mMethod), params);
  mPlanet->unlock();
  
  *mOutput << res << std::endl;
}

void Command::execute_command()
{
  oscit::Object * obj;
  Value res;
  Value params = get_params();
  if (mMethod == "set_lib_path") {
    mPlanet->lock();
      res = mPlanet->call(std::string(CLASS_ROOT).append("/lib_path"),params);
    mPlanet->unlock();
  } else if (mMethod == "quit" || mMethod == "q") {
    mPlanet->lock();
      mPlanet->quit();
      mQuit = true;
    mPlanet->unlock();    
  } else {
    mPlanet->lock();
      // FIXME: Group scope
      obj = mPlanet->find(std::string("/").append(mMethod));
      if (params.is_nil() && obj->type() == H("Node"))
        res = mPlanet->call(std::string("/").append(mMethod).append("/#inspect"));
      else
        res = obj->trigger(params);
    mPlanet->unlock();
  }
  if (!res.is_nil()) *mOutput << res << std::endl;
  /*
  TODO: these methods should exist in root...
  
  if (mPlanet->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
    
  } else if (mMethod == "quit" || mMethod == "q") {
    mPlanet->quit();
    mQuit = true;
  } else if (mMethod == "set_lib_path") {
    std::string path;
    if (!mParameters.get(&path)) {
      *mOutput << "Could not set library path (no parameter).\n";
    } else {
      Class::set_lib_path(path);
      *mOutput << "Library path set to '" << path << "'.\n";
    }
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  */
}

void Command::clear() 
{
  mTokenIndex = 0;
  mAction     = CmdNoAction;
  mVar        = "";
  mClass      = "";
  mParamString = "";
  mFromPort = "";
  mToPort   = "";
}
