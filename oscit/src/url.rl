/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "oscit/url.h"

#include <stdlib.h> // atoi
#include <string.h> // strlen

#include <sstream>


/** Ragel parser definition to parse urls. */
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
  hostname        = hostpart ('.' $str_a hostpart)*;
  dquote_content  = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  squote_content  = ([^'\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  string          = ('"' dquote_content* '"' | '\'' squote_content* '\'');
  port            = digit+;
  host_or_service = (hostname %set_hostname (':' port $str_a %set_port) |
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
  full_url_ = url;
  std::string str_buf;
  // =============== Ragel job ==============

  int cs;
  const char * p  = url;
  const char * pe = url + strlen(p) + 1;

  %% write init;
  %% write exec;
}

} // oscit
