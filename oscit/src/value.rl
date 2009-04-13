/** Ragel parser definition to create Values from JSON. */

#define MAX_NUM_BUFFER_SIZE 50
//#define DEBUG_PARSER

#include "oscit/values.h"
#include <iostream>
#include <ostream>

namespace oscit {

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

%%{
  machine json;

  action num_a {
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(json);
    }
#ifdef DEBUG_PARSER
printf("%c_",fc);
#endif
    num_buf[num_buf_i] = fc; /* append */
    num_buf_i++;
  }

  action str_a {
     // append a char to build a std::string
#ifdef DEBUG_PARSER
    printf("%c-",fc);
#endif
    if (fc)
      str_buf.append(&fc, 1); /* append */
  }

  action number {
    // become a RealValue
    num_buf[num_buf_i+1] = '\0';
    set(atof(num_buf));
  }

  action string {
    // become a StringValue
    set(str_buf);
    str_buf = "";
  }

  action hash_value {
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p);
    set(str_buf, tmp_val);
    
    str_buf = "";
    fhold;
  }
  
  action list_value {
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p);
    push_back(tmp_val);
    if (*(p-1) == ',') fhold; // hold the ',' separator
    
    fhold; // eaten by >list_value sub-action
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
  }
  
  action nil {
    // become a NilValue
    set_type(NIL_VALUE);
  }
  
  action debug {
    printf("%c?",fc);
  }
  
  action debug2 {
    printf("%c!",fc);
  }
  
  ws        = ' ' | '\t' | '\n';
  end       = ws  | '\0' | '}' | ',' | ']';  # we need '}' and ']' to finish value when embedded in hash: {one:1.34}
  char      = ([^"\\] | '\n') $str_a | ('\\' (any | '\n') $str_a);
  word      = (alpha [^ \t\n:]*) $str_a;
  real      = ([\-+]? $num_a ('0'..'9' digit* '.' digit+) $num_a );
  integer   = ([\-+]? $num_a ('0'..'9' digit*) $num_a );
            
  number    = real | integer;
  string    = ('"' char* '"') | word | '/' $str_a char*; # special case for urls
  hash      = '{'? (ws* string ':' >hash_value)+ ws* '}'? | '{' ws* '}';
  nil       = 'null';
  true      = 'true';
  false     = 'false';
  list      = ('[' >list_value ws* (',' >list_value ws*)* ']') $debug;
  
  main     := ws* (string %string | number %number | hash %hash | list %list | nil %nil) end; # | true | false;
  
}%%

// transition table
%% write data;

/** This is a crude JSON parser. */
size_t Value::build_from_json(const char *json, bool lazy_allowed) {
  std::cout << "\nbuild_from_json:\"" << json << "\"\n";
  char num_buf[MAX_NUM_BUFFER_SIZE + 1];
  unsigned int num_buf_i = 0;
  std::string str_buf;
  Value tmp_val;   // used when building Hash or List
  
  // =============== Ragel job ==============
  
  int cs;
  const char * p  = json;
  const char * pe = json + strlen(p) + 1;
  
  %% write init;
  %% write exec;
  
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