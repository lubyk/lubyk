#include "values.h"

/** Ragel parser definition to create Values from JSON. */
#define MAX_NUM_BUFFER_SIZE 50
// #define DEBUG_PARSER

%%{
  machine json;

  action num_a {
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(pStr);
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
    num_buf[num_buf_i+1] = '\0';
    Number(atof(num_buf)).set(*this);
  }

  action string {
    String(str_buf).set(*this);
    str_buf = "";
  }

  action hash_value {
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.from_string(p);
    tmp_h.set_key(str_buf, tmp_val);
    if (p == pe)
      tmp_h.set(*this);
    
    str_buf = "";
    fhold;
  }

  action hash {
    tmp_h.set(*this);
  }

  action bang {
    gBangValue.set(*this);
  }

  
  ws        = ' ' | '\t' | '\n';
  end       = ws  | '\0' | '}';  # we need '}' to finish value when embedded in hash: {one:1.34}
  char      = ([^"\\] | '\n' | ( '\\' (any | '\n') )) $str_a;
  word      = (alpha [^ \t\n:]*) $str_a;
  real      = ([\-+]? $num_a ('0'..'9' digit* '.' digit+) $num_a );
  integer   = ([\-+]? $num_a ('0'..'9' digit*) $num_a );
            
  number    = real | integer;
  string    = ('"' char* '"') | word | '/' $str_a char*; # special case for urls
  hash      = '{'? (ws* string ':' >hash_value)+ ws* '}'? | '{' ws* '}';
  bang      = 'Bang!';
#  array   = '[]' | '[' ($value)* $array_element ']';
  
  main     := ws* (string %string | number %number | hash %hash | bang %bang) end; # | array | true | false | null;
  
}%%

// transition table
%% write data;

/** This is a crude JSON parser. */
size_t Value::from_string(const char * pStr)
{
  char num_buf[MAX_NUM_BUFFER_SIZE + 1];
  unsigned int num_buf_i = 0;
  std::string str_buf;
  Value tmp_val;   // used when building Hash
  Hash  tmp_h;     // used to build tmp Hash
  
  // =============== Ragel job ==============
  
  int cs;
  const char * p  = pStr;
  const char * pe = pStr + strlen(p) + 1;
  
  %% write init;
  %% write exec;
  
  return p - pStr;
}

std::ostream &operator<<(std::ostream &out_stream, const Value &val) {
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << val.r;
      break;
    case ERROR_VALUE:
      out_stream << val.error_code() << val.error_message();
      break;
    case STRING_VALUE:
      out_stream << val.s;
      break;
    case NIL_VALUE:
      // ???? out_stream << val.s;
      break;
    case LIST_VALUE:
      size_t sz = val.size();
      for (size_t i = 0; i < sz; ++i) {
        out_stream << val[i];
      }
      break;
    default:
      // ????
  }
  return out_stream;
}


/** Display number inside stream. */
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

/** Display number inside stream. */
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

/** Display MatrixData as json. */
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

/** Display CharMatrixData as json. */
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