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

#include "oscit/values.h"

//#define DEBUG_PARSER

#include <string.h>  // strlen
#include <stdlib.h>  // atof

#include <iostream>
#include <sstream>


/** Ragel parser definition to create Values from JSON. */
namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

Value gNilValue('N');
Value gTrueValue(1.0);
Value gFalseValue(0.0);
Value gEmptyValue;
Hash  gEmptyHash(1);

static std::string escape(const std::string &string) {
  std::string res;
  size_t len = 0;
  const char *last_append = string.c_str();
  const char *ptr = string.c_str();
  while (*ptr) {
    if (*ptr == '"') {
      // append \"
      if (len) {
        res.append(last_append, len);
        last_append = ptr + 1;
        len = 0;
      }
      res.append("\\\"");
      ++ptr;
    } else if (*ptr == '\\') {
      ++len;
      ++ptr;
      if (*ptr) {
        // in case string ends with "\"
        ++ptr;
        ++len;
      }
    } else {
      // append
      ++len;
      ++ptr;
    }
  }

  if (len) res.append(last_append, len);
  return res;
}

std::ostream &operator<<(std::ostream &out_stream, const Value &val) {
  val.to_stream(out_stream);
  return out_stream;
}

void Value::to_stream(std::ostream &out_stream, bool lazy) const {
  size_t sz;
  switch (type()) {
    case REAL_VALUE:
      out_stream << r;
      break;
    case ERROR_VALUE:
      out_stream << "\"" << error_code() << " " << escape(error_message()) << "\"";
      break;
    case STRING_VALUE:
      if (lazy) {
        out_stream << str();
      } else {
        out_stream << "\"" << escape(str()) << "\"";
      }
      break;
    case HASH_VALUE:
      if (lazy) {
        hash_->to_stream(out_stream, true);
      } else {
        out_stream << "{" << *hash_ << "}";
      }
      break;
    case MATRIX_VALUE:
      out_stream << "\"Matrix " << matrix_->rows() << "x" << matrix_->cols() << "\"";
      break;
    case MIDI_VALUE:
      out_stream << "\"MidiMessage " << *midi_message_ << "\"";
      break;
    case LIST_VALUE:
      sz = size();
      if (!lazy) out_stream << "[";
      for (size_t i = 0; i < sz; ++i) {
        if (i > 0) out_stream << ", ";
        out_stream << this->operator[](i);
      }
      if (!lazy) out_stream << "]";
      break;
    case EMPTY_VALUE:
      break; // nothing
    case ANY_VALUE: /* continue */
    case NIL_VALUE: /* continue */
    default:
      out_stream << "null";
  }
}

Json Value::to_json() const {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  return (Json)os.str();
}

Json Value::lazy_json() const {
  std::ostringstream os(std::ostringstream::out);
  to_stream(os, true);
  return (Json)os.str();
}

Value &Value::push_back(const Value& val) {
  if (!val.is_empty()) {
    if (is_list()) {
      list_->push_back(val);
    } else if (is_empty() && val.is_list()) {
      set_type(LIST_VALUE);
      list_->push_back(val);
    } else if (is_empty()) {
      set(val);
    } else {
      // copy self as first element
      Value original(*this);
      set_type(LIST_VALUE);
      list_->push_back(original);
      list_->push_back(val);
    }
  }
  return *this;
}

Value &Value::push_front(const Value& val) {
  if (!val.is_empty()) {
    if (is_list()) {
      list_->push_front(val);
    } else if (is_empty()) {
      set(val);
    } else {
      // copy self as first element
      Value original(*this);
      set_type(LIST_VALUE);
      list_->push_back(val); // push_back is faster
      list_->push_back(original);
    }
  }
  return *this;
}

static void s_deep_merge(const Value a, const Value b) {
  HashIterator it, end = b.end();
  std::string key;
  Hash *res_hash = a.hash_;
  Value val_a, val_b;

  for (it = b.begin(); it != end; ++it) {
    key = *it;
    if (!b.hash_->get(key, &val_b)) continue; // this should never happen, just in case ;-)

    if (a.hash_->get(key, &val_a)) {
      // existing key
      if (val_a.is_hash() && val_b.is_hash()) {
        // deep merge
        s_deep_merge(val_a, val_b);
      } else if (val_b.is_nil()) {
        // remove
        res_hash->remove(key);
      } else {
        // update
        res_hash->set(key, val_b);
      }
    } else {
      // new key
      res_hash->set(key, val_b);
    }
  }
}

void Value::deep_merge(const Value &other) {
  if (!is_hash() || !other.is_hash()) return;
  s_deep_merge(*this, other);
}

///////////////// ====== JSON PARSER ========= /////////////
%%{
  machine json;

  action str_a {
     // append a char to build a std::string
    DEBUG(printf("%c-",fc));
    if (fc)
      str_buf += fc; /* append */
  }

  action number {
    // become a RealValue
    tmp_val.set(atof(str_buf.c_str()));
    DEBUG(printf("[number %f/%s/%s\n]", tmp_val.r, str_buf.c_str(), tmp_val.to_json().c_str()));
    str_buf = "";
  }

  action string {
    // become a StringValue
    tmp_val.set(str_buf);
    DEBUG(printf("[string %s]\n", tmp_val.to_json().c_str()));
    str_buf = "";
  }

  action hash_value {
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p, true);
    set(str_buf, tmp_val);
    fhold;
    DEBUG(printf("[hash_value \"%s\":%s]\n", str_buf.c_str(), tmp_val.to_json().c_str()));
    DEBUG(printf("[continue \"%s\"]\n",p));

    str_buf = "";
  }

  action list_value {
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p, true);
    push_back(tmp_val);
    if (*(p-1) == ',') fhold; // hold the ',' separator

    DEBUG(printf("[%p:list_value %s ==> %s/%s]\n", this, tmp_val.to_json().c_str(), to_json().c_str(), p));
    fhold; // eaten by >list_value sub-action
  }

  action lazy_list {
    // we have a value in tmp that should be changed into a list [tmp]
    DEBUG(printf("[%p:lazy_list %s]\n", this, tmp_val.to_json().c_str()));
    push_back(tmp_val);
  }

  action empty_hash {
    DEBUG(printf("[%p:empty_hash %s]\n", this, tmp_val.to_json().c_str()));
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }

  action list {
    if (!is_list()) set_type(LIST_VALUE);

    DEBUG(printf("[%p:list %s]\n", this, p));
    // FIXME: how to avoid 'return' by telling parsing to stop ?
    return p - json + 1;
  }

  action nil {
    // become a NilValue
    DEBUG(printf("[nil]\n"));
    tmp_val.set_type(NIL_VALUE);
  }

  action set_from_tmp {
    DEBUG(printf("[set_from_tmp %s]\n", tmp_val.to_json().c_str()));
    if (!is_list() && !is_hash()) *this = tmp_val;
  }

  ws        = ' ' | '\t' | '\n';
  end       = ws  | '\0' | '}' | ',' | ']';  # we need '}' and ']' to finish value when embedded in hash: {one:1.34}
  dquote_content = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  squote_content = ([^'\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  word      = ws* (alpha [^ \t\n:]*) $str_a;
  real      = ws* ([\-+]? $str_a ('0'..'9' digit* '.' digit+) $str_a );
  integer   = ws* ([\-+]? $str_a ('0'..'9' digit*) $str_a );
  nil       = 'null';
  true      = 'true';
  false     = 'false';
  number    = real | integer;
  string    = ws* ('"' dquote_content* '"' | '\'' squote_content* '\'');

  hash_content = (string | word | integer) ':' >hash_value;

  strict    = ws* '[' >list_value (',' >list_value)* ']' $list    |
              ws* '{' hash_content (','? hash_content)* '}' |
              ws* '{' ws* '}'        %empty_hash |
                      string             %string |
                      number             %number |
                      nil                %nil;

  lazy_list_content = strict %lazy_list ',' >list_value (',' >list_value)*;

  lazy      = lazy_list_content          %list   |
              hash_content (','? hash_content)*  |
              strict;

  main_strict := strict %set_from_tmp end;
  main_lazy   := lazy   %set_from_tmp end;

}%%

// transition table
%% write data;

/** This is a crude JSON parser. */
size_t Value::build_from_json(const char *json, bool strict_mode) {
  DEBUG(printf("\nbuild_from_json:\"%s\"\n",json));
  std::string str_buf;
  Value tmp_val;
  set_empty(); // clear
  // =============== Ragel job ==============

  int cs;
  const char * p  = json;
  const char * pe = json + strlen(p) + 1;

  %% write init;

  if (strict_mode) {
    cs = json_en_main_strict;
  } else {
    cs = json_en_main_lazy;
  }

  %% write exec;
  if (p != pe) --p;

  return p - json;
}

} // oscit


/*

// old stuff, remove if we decide we do not need to stream matrix data as json...

template<>
void MatrixData::to_stream(std::ostream& pStream) const
{
  char buffer[20];
  if (size() == 0) {
#ifdef _TESTING_
    if (sShowId)
      pStream << "<" << type_name() << "[" << mId << "] 0>";
    else
#endif
    pStream << "<" << type_name() << " 0>";

  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    if (sShowId)
      snprintf(buffer, 20, "<%s[%lu] [ % .2f", type_name(), mId, data[start]);
    else
#endif
    snprintf(buffer, 20, "<%s [ % .2f", type_name(), data[start]);

    pStream << buffer;
    for (size_t i= start+1; i < start+sz; i++) {
      snprintf(buffer, 20, " % .2f", data[i]);
      pStream << buffer;
    }
    pStream << " ], " << mRowCount << "x" << mColCount << ">";
  }
}


template<>
void CharMatrixData::to_stream(std::ostream& pStream) const
{
  if (size() == 0) {
#ifdef _TESTING_
    if (sShowId)
      pStream << "<" << type_name() << "[" << mId << "] 0>";
    else
#endif
    pStream << "<" << type_name() << " 0>";

  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    if (sShowId)
      pStream << "<" << type_name() << "[" << mId << "]" << " [ " << data[start];
    else
#endif
    pStream << "<" << type_name() << " [ " << data[start];

    for (size_t i= start+1; i < start+sz; i++) {
      pStream << " " << data[i];
    }
    pStream << " ], " << mRowCount << "x" << mColCount << ">";
  }
}


template<>
void MatrixData::to_json(std::ostream& pStream) const
{
  char buffer[20];

  size_t sz = size();
  pStream << "[";

  for (size_t i = 0; i < sz; i++) {
    if (i > 0) pStream << ",";
    snprintf(buffer, 20, "%.2f", data[i]);
    pStream << buffer;
  }

  pStream << "]";
}


template<>
void CharMatrixData::to_json(std::ostream& pStream) const
{
  size_t sz = size();
  pStream << "[";

  for (size_t i = 0; i < sz; i++) {
    if (i > 0) pStream << ",";
    pStream << data[i];
  }

  pStream << "]";
}
*/
