#include "oscit/mapper.h"

#include <stdlib.h> // atof

#include <string>
#include <iostream>

namespace oscit {

//#define DEBUG_PARSER

Mapper::Mapper() : map_(200), reverse_map_(200) {}

Mapper::Mapper(size_t hash_table_size) : map_(hash_table_size), reverse_map_(hash_table_size) {}

Mapper::~Mapper() {
  clear();
}

void Mapper::clear() {
  map_.clear();
  reverse_map_.clear();
}

bool Mapper::set_map(const std::string &source_url, Real source_min, Real source_max,
             const std::string &target_url, Real target_min, Real target_max) {
  if (source_min == source_max) {
    // TODO: record line, better error reporting
    error_ = std::string("Source min and max cannot be the same value !");
    return false;
  }

  if (target_min == target_max) {
    // TODO: record line, better error reporting
    error_ = "Target min and max cannot be the same value (used for reverse mapping) !";
    return false;
  }

  map_.set(source_url, MapElement(target_url, source_min, source_max, target_min, target_max));
  reverse_map_.set(target_url, MapElement(source_url, target_min, target_max, source_min, source_max));
  return true;
}


bool Mapper::map(const std::string &source, Real value, std::string *target, Real *target_value) {
  const MapElement *res;
  if (!map_.get(source, &res)) return false;
  *target       = res->target_url();
  *target_value = res->scale(value);
  return true;
}

bool Mapper::reverse_map(const std::string &source, Real value, std::string *target, Real *target_value) {
  const MapElement *res;
  if (!reverse_map_.get(source, &res)) return false;
  *target        = res->target_url();
  *target_value  = res->scale(value);
  return true;
}

//// Mapping parser ///////

%%{
  machine mapper;

  action str_a {
   #ifdef DEBUG_PARSER
     printf("_%c_",fc);
   #endif
   str_buf += fc;
  }

  action source_url {
   source_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_url " << source_url << "]" << std::endl;
   #endif
  }

  action target_url {
   target_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_url " << target_url << "]" << std::endl;
   #endif
  }

  action source_min {
   source_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_min " << source_min << "]" << std::endl;
   #endif
  }

  action source_max {
   source_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_max " << source_max << "]" << std::endl;
   #endif
  }

  action target_min {
   target_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_min " << target_min << "]" << std::endl;
   #endif
  }

  action target_max {
   target_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_max " << target_max << "]" << std::endl;
   #endif
  }

  action set_map {
   #ifdef DEBUG_PARSER
     std::cout << "[set_map " << source_url << " [" << source_min << ", " << source_max << "]" << " --> " <<
                                 target_url << " [" << target_min << ", " << target_max << "]" << std::endl;
   #endif
   if (!set_map(source_url, source_min, source_max, target_url, target_min, target_max)) return false;

   source_url = "";
   source_min = 0.0;
   source_max = 0.0;
   target_url = "";
   target_min = 0.0;
   target_max = 0.0;
  }

  action error {
    fhold; // move back one char
    char error_buffer[10];
    snprintf(error_buffer, 9, "%s", p);
    std::cerr << "# Syntax error near '" << error_buffer << "'." << std::endl;

    source_url = "";
    source_min = 0.0;
    source_max = 0.0;
    target_url = "";
    target_min = 0.0;
    target_max = 0.0;

    state = fentry(main);
    fgoto eat_line; // eat the rest of the line and continue parsing
  }

  action comment {
    state = cs;
    fgoto eat_line;
  }

  eat_line := [^\n]* '\n' @{ fgoto *state; printf("comment: [%s:%i]\n", p, cs);};

  ws        = ' ' | '\t' | '\n' ;
  wsc       = ws* ('#' $comment)? ws*;
  dquote_content = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  squote_content = ([^'\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  word      = ws* ((alpha | '/') [^ \t\n:]*) $str_a;
  string    = ws* ('"' dquote_content* '"' | '\'' squote_content* '\'' | word);

  real      = ws* ([\-+]? $str_a ('0'..'9' digit* '.' digit+) $str_a );
  integer   = ws* ([\-+]? $str_a ('0'..'9' digit*) $str_a );
  number    = real | integer;

  map_entry = string %source_url ws wsc '[' number %source_min ws* ',' number %source_max ws* ']' wsc '-'+ '>'
              wsc string %target_url ws wsc '[' number %target_min ws* ',' number %target_max ws* ']' %set_map;

  main := (map_entry '\n' | ws+ | ws* ('#' $comment) ws*)+ '\0' $err(error);

}%%

%% write data;

bool Mapper::parse(const std::string &definitions) {
 std::string script(definitions);
 script.append("\n");
 int cs, state;
 const char * p  = script.data(); // data pointer
 const char * pe = p + script.size(); // past end
 const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
 #ifdef DEBUG_PARSER
   printf("Parsing: \"%s\"", script.c_str());
 #endif

 // source state, target state
 std::string source_url;
 std::string target_url;
 std::string str_buf;
 Real source_min, source_max, target_min, target_max;

 %% write init;

 %% write exec;

 return true;
}

}  // oscit
