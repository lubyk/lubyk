/** Ragel parser definition to parse urls. */
#include "oscit/url.h"
#include <sstream>
#include <stdlib.h> // atoi
#include <string.h> // strlen

#define MAX_NUM_BUFFER_SIZE 20
//#define DEBUG_PARSER


namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

std::ostream &operator<<(std::ostream &out_stream, const Url &url) {
  out_stream << url.protocol_ << "://" << url.host_;
  if (url.port_ != 0) {
    out_stream << ":" << url.port_;
  }
  out_stream << url.path_;
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

  action num_a {
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      fprintf(stderr, "Buffer overflow when parsing port !\n");
      // stop parsing
      return;
    }
    DEBUG(printf("%c_",fc));
    num_buf[num_buf_i] = fc; /* append */
    num_buf_i++;
  }

  action str_a {
    DEBUG(printf("%c-",fc));
    if (fc) str_buf.append(&fc, 1); /* append */
  }
  
  action set_protocol {
    protocol_ = str_buf;
    str_buf = "";
    DEBUG(printf("[protocol %s\n]", protocol_.c_str()));
  }
  
  action set_host {
    host_ = str_buf;
    str_buf = "";
    DEBUG(printf("[host %s\n]", host_.c_str()));
  }
  
  action set_port {
    port_ = atoi(str_buf.c_str());
    str_buf = "";
    DEBUG(printf("[port %i\n]", port_));
  }

  action set_path {
    path_ = str_buf;
    str_buf = "";
    DEBUG(printf("[path %s\n]", path_.c_str()));
  }
  
  action clear_path {
    path_ = "";
  }
  
  ws        = ' ' | '\t' | '\n';
  end       = ws  | '\0';
  protocol  = alpha+;
  host      = (alpha | digit | '.')+;
  port      = digit+;
  path      = (alpha | digit | '/' | '.')+;
  
  main     := ((protocol $str_a  %set_protocol '://'
              host $str_a %set_host
              (':' port $str_a %set_port)?
              ('/' path) $str_a %set_path) |
              path $str_a %set_path) 
              end;
  
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
