#include "text_command.h"
#include "rubyk.h"

//#define DEBUG_PARSER

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

%%{
  machine text_command;
  write data noerror;
}%%

void TextCommand::initialize() {
  int cs;
  
  silent_     = false;    
  clear();
  %% write init;
  current_state_ = cs;
}

void TextCommand::do_listen() {
  char buffer[1024];
  char * line = buffer;
  
  if (!silent_) *output_ << "Welcome to rubyk !\n\n";
    
  clear();

  while(should_run() && getline(&line,1023)) {
    parse(line);
    parse("\n");
    if (should_run()) saveline(line); // command was not a 'quit'
    freeline(line);
  }
}

void TextCommand::parse(const std::string &string) {
  const char *p  = string.data();     // data pointer
  const char *pe = p + string.size(); // past end
  const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
  int cs = current_state_;            // restore machine state
  
  DEBUG(printf("parse:\"%s\"\n",string.c_str()));
  
  %%{
    action a {
      DEBUG(printf("_%c_",fc));
      token_ += fc; /* append */
    }
    
    action params {
      // FIXME: this is a temporary hack until we sub parse with Value...
      parameter_string_ += fc;
    }
    
    action set_var    { set_from_token(var_);}
    
    action set_method { set_from_token(method_);}
  
    action set_class  { set_from_token(class_);}
  
    action set_value  { set_from_token(value_);}
    
    action set_from      { from_node_     = var_; }
    
    action set_from_port { from_port_ = var_; }
    
    action set_to_port   { to_port_   = var_; }
    
    action add_param_value { 
      add_value_from_token(); 
    }
    
    action create_link {
      to_node_   = var_;
      create_link();
    }
    
    action remove_link {
      to_node_   = var_;
      remove_link();
    }

    action create_instance { create_instance(); }
    
    action execute_method { execute_method(); }
    
    action execute_class_method { execute_class_method(); }
    
    action execute_command { execute_command(); }
    
    action debug { printf("[%c]", p[0]); }
    
    action prompt {
      clear();
    }
    
    action error {
      fhold; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "Syntax error near '" << error_buffer << "'." << std::endl;
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
  current_state_ = cs;
}

void TextCommand::set_from_token(std::string &string) {
  DEBUG(if (&string == &value_) std::cout << "[val " << token_ << "]" << std::endl);
  DEBUG(if (&string == &var_)   std::cout << "[var " << token_ << "]" << std::endl);
  DEBUG(if (&string == &class_) std::cout << "[cla " << token_ << "]" << std::endl);
  
  string = token_;
  token_ = "";
}

void TextCommand::create_instance() {
  Value params(Json(parameter_string_.c_str()));
  ListValue list;
  names_to_urls();
  
  DEBUG(std::cout << "NEW "<< var_ << " = " << class_ << "(" << params << ")");
  list.push_back(var_);
  list.push_back(params);
  
  Value res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/new"), list);
  
  if (res.is_string() && !silent_) {
    res = root_->call("/.inspect", res);
    *output_ << res << std::endl;
  } else if (!res.is_string()) {
    *output_ << res << std::endl;
  } 
}

void TextCommand::change_link(char op) {
  names_to_urls();
  
  DEBUG(std::cout << op << " LINK " << from_node_ << "." << from_port_ << "=>" << to_port_ << "." << to_node_ << std::endl);
  ListValue list;
  Value res;
  
  if (from_port_ == "") {
    list.push_back(std::string(from_node_).append("/out"));
  } else {
    list.push_back(std::string(from_node_).append("/out/").append(from_port_));
  }
  if (to_port_ == "") {
    list.push_back(std::string(to_node_).append("/in"));
  } else {
    list.push_back(std::string(to_node_).append("/in/").append(to_port_));
  }
  if (op == 'c') {
    res = root_->call(LINK_URL, list);
  } else {
    res = root_->call(UNLINK_URL, list);
  }
  
  if (!silent_) *output_ << res << std::endl;
}

void TextCommand::execute_method() {
  Value res;
  // why doesn't this work ? Value params(Json(parameter_string_));
  Value params = Value(Json(parameter_string_));
  names_to_urls();
  
  DEBUG(std::cout << "METHOD " << var_ << "." << method_ << "(" << params << ")" << std::endl);
  
  if (method_ == "set") {
    // TODO: should 'set' live in normal tree space ?
    Object *target = root_->object_at(var_);
    if (target) {
      target->lock();
        res = target->set(params);
      target->unlock();
    } else {
      res = ErrorValue(NOT_FOUND_ERROR, var_);
    }
  } else {
    if (method_ == "b") method_ = "bang";
    var_.append("/").append(method_);
    res = root_->call(var_, params);
  }
  
  if (!silent_) *output_ << res << std::endl;
}

void TextCommand::execute_class_method() {
  Value res;
  Value params = Value(Json(parameter_string_));
  
  DEBUG(std::cout << "CLASS_METHOD " << std::string(CLASS_URL).append("/").append(class_).append("/").append(method_) << "(" << params << ")" << std::endl);
  res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/").append(method_), params);
  
  if (!silent_) *output_ << res << std::endl;
}

void TextCommand::execute_command() {
  Value res;
  Value params = Value(Json(parameter_string_));

  DEBUG(std::cout << "CMD " << method_ << "(" << params << ")" << std::endl);
  if (method_ == "set_lib_path") {
    res = root_->call(std::string(CLASS_URL).append("/lib_path"), params);
  } else if (method_ == "quit" || method_ == "q") {
    res = root_->call(QUIT_URL);
  } else {  
    method_.insert(0, current_directory_);
    res = root_->call(method_, params);
  }
  
  if (!silent_) *output_ << res << std::endl;
}

void TextCommand::clear() {
  token_      = "";
  var_        = "";
  class_      = "";
  parameter_string_ = "";
  from_port_  = "";
  to_port_    = "";
}
