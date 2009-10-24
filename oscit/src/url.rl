/** Ragel parser definition to parse urls. */
#include "oscit/url.h"
#include <sstream>
#include <stdlib.h> // atoi
#include <string.h> // strlen

// #define DEBUG_PARSER

namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

std::ostream &operator<<(std::ostream &out_stream, const Url &url) {
  out_stream << url.location_ << url.path_;
  return out_stream;
}

void Url::rebuild_full_url() {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  full_url_ = os.str();
}

///////////////// ====== URL PARSER ========= /////////////
%%{
  machine url;

  action str_a {
    DEBUG(printf("%c-",fc));
    if (fc) str_buf.append(&fc, 1); /* append */
  }

  action set_protocol {
    location_.protocol_ = str_buf;
    str_buf = "";
    DEBUG(printf("[protocol %s\n]", location_.protocol_.c_str()));
  }

  action set_hostname {
    if (location_.protocol_ == "") {
      location_.protocol_ = DEFAULT_PROTOCOL;
    }
    location_.name_ = str_buf;
    location_.reference_by_hostname_ = true;
    str_buf = "";
    DEBUG(printf("[host %s\n]", location_.name_.c_str()));
  }

  action set_service_name {
    if (location_.protocol_ == "") {
      location_.protocol_ = DEFAULT_PROTOCOL;
    }
    location_.name_ = str_buf;
    location_.reference_by_hostname_ = false;
    str_buf = "";
    DEBUG(printf("[service \"%s\"\n]", location_.name_.c_str()));
  }

  action set_port {
    location_.port_ = atoi(str_buf.c_str());
    str_buf = "";
    DEBUG(printf("[port %i\n]", location_.port_));
  }

  action set_path {
    path_ = str_buf;
    str_buf = "";
    DEBUG(printf("[path %s\n]", path_.c_str()));
  }

  action clear_path {
    path_ = "";
  }

  ws              = ' ' | '\t' | '\n';
  end             = ws  | '\0';
  protocol        = alpha+;
  hostpart        = (alpha | digit)+ $str_a;
  hostname        = hostpart ('.' $str_a hostpart) ('.' $str_a hostpart)*;
  dquote_content  = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  squote_content  = ([^'\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  string          = ('"' dquote_content* '"' | '\'' squote_content* '\'');
  port            = digit+;
  host_or_service = (hostname %set_hostname (':' port $str_a %set_port)? |
                    string %set_service_name);
  path            = (alpha | digit | '/' | '.' | '_' )+;

  main           := ((protocol $str_a  %set_protocol '://')? host_or_service
                     ('/' path) $str_a %set_path |
                     path $str_a %set_path
                    ) end;

}%%

// transition table
%% write data;

/** This is a crude JSON parser. */
void Url::parse(const char *url) {
  std::string str_buf;
  // =============== Ragel job ==============

  int cs;
  const char * p  = url;
  const char * pe = url + strlen(p) + 1;

  %% write init;
  %% write exec;
}

} // oscit
