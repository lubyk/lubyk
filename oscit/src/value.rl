/** Ragel parser definition to create Values from JSON. */

#define MAX_NUM_BUFFER_SIZE 50
//#define DEBUG_PARSER

#include "oscit/values.h"
#include "oscit/list.h"
#include <iostream>
#include <ostream>

namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

std::ostream &operator<<(std::ostream &out_stream, const Value &val) {
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << val.r;
      break;
    case ERROR_VALUE:
      out_stream << "\"" << val.error_code() << " " << val.error_message() << "\"";
      break;
    case STRING_VALUE:
      out_stream << "\"" << val.str() << "\"";
      break;
    case HASH_VALUE:
      out_stream << "{" << *val.hash_ << "}";
      break;
    case MATRIX_VALUE:
      out_stream << "\"Matrix " << val.matrix_->rows << "x" << val.matrix_->cols << "\"";
      break;
    case NIL_VALUE:
      out_stream << "null";
      break;
    case LIST_VALUE:
      size_t sz = val.size();
      out_stream << "[";
      for (size_t i = 0; i < sz; ++i) {
        if (i > 0) out_stream << ", ";
        out_stream << val[i];
      }
      out_stream << "]";
      break;
    default:
      ;// ????
  }
  return out_stream;
}

Json Value::to_json() const {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  return (Json)os.str();
}

Value &Value::push_back(const Value& val) {
  if (is_list()) {
    list_->push_back(val);
  } else if (is_nil() && !val.is_list()) {
    set(val);
  } else {
    if (!is_nil()) {
      // copy self as first element
      Value original(*this);
      set_type(LIST_VALUE);
      push_back(original);
    } else {
      set_type(LIST_VALUE);
    }

    list_->push_back(val);
  }
  return *this;
}

Value &Value::push_front(const Value& val) {
  if (is_nil()) {
    set(val);
  } else {
    if (!is_list()) {
      Value tmp(*this);
      set_type(LIST_VALUE);
      if (!tmp.is_nil()) push_back(tmp);
    }

    list_->push_front(val);
  }
  return *this;
}

///////////////// ====== JSON PARSER ========= /////////////
%%{
  machine json;

  action num_a {
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(json);
    }
    DEBUG(printf("%c_",fc));
    num_buf[num_buf_i] = fc; /* append */
    num_buf_i++;
  }

  action str_a {
     // append a char to build a std::string
    DEBUG(printf("%c-",fc));
    if (fc)
      str_buf.append(&fc, 1); /* append */
  }

  action number {
    // become a RealValue
    num_buf[num_buf_i] = '\0';
    tmp_val.set(atof(num_buf));
    DEBUG(printf("[number %f/%s/%s\n]", tmp_val.r, num_buf, tmp_val.to_json().c_str()));
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
    p += tmp_val.build_from_json(p);
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

  action hash {
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }
  
  action list {
    // become an empty list
    if (!is_list()) {
      set_type(LIST_VALUE);
    }
    DEBUG(printf("[%p:list %s]\n", this, p));
    // FIXME: how to avoid 'return' by telling parsing to stop ?
    return p - json + 1;
  }
  
  action nil {
    // become a NilValue
    set_type(NIL_VALUE);
  }
  
  action set_from_tmp {
    DEBUG(printf("[set_from_tmp %s]\n", tmp_val.to_json().c_str()));
    if (!is_list() && !is_hash()) *this = tmp_val;
  }
  
  ws        = ' ' | '\t' | '\n';
  end       = ws  | '\0' | '}' | ',' | ']';  # we need '}' and ']' to finish value when embedded in hash: {one:1.34}
  char      = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  word      = ws* (alpha [^ \t\n:]*) $str_a;
  real      = ws* ([\-+]? $num_a ('0'..'9' digit* '.' digit+) $num_a );
  integer   = ws* ([\-+]? $num_a ('0'..'9' digit*) $num_a );
  nil       = 'null';
  true      = 'true';
  false     = 'false';
  number    = real | integer;
  string    = ws* ('"' char* '"');
  
  hash_content = ((string | word) ':' >hash_value)+;
  
  strict    = ws* '[' >list_value (',' >list_value)* ']' $list |
              ws* '{' hash_content   '}' %hash   |
                      string             %string |
                      number             %number |
                      nil                %nil;
                      
  lazy_list_content = strict %lazy_list ',' >list_value (',' >list_value)*;

  lazy      = lazy_list_content          %list   |          
              hash_content               %hash   |
              strict;  
  
  main_strict := strict %set_from_tmp end;
  main_lazy   := lazy   %set_from_tmp end;
  
}%%

// transition table
%% write data;

/** This is a crude JSON parser. */
size_t Value::build_from_json(const char *json, bool strict_mode) {
  DEBUG(printf("\nbuild_from_json:\"%s\"\n",json));
  char num_buf[MAX_NUM_BUFFER_SIZE + 1];
  unsigned int num_buf_i = 0;
  std::string str_buf;
  Value tmp_val;
  set_type(NIL_VALUE); // clear
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