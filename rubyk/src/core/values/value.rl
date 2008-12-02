#include "values.h"

/** Ragel parser definition to create Values from JSON. */
#define MAX_NUM_BUFFER_SIZE 50

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
      printf("_%c_",fc);
    #endif
      num_buf[num_buf_i] = fc; /* append */
    num_buf_i++;     
  }

  action str_a {
     // append a char to build a std::string
#ifdef DEBUG_PARSER
    printf("_%c_",fc);
#endif
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

  action hash {
    tmp_h.set(*this);
  }

  action hash_value {
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.from_string(p);
    tmp_h.set_key(str_buf, tmp_val);
    str_buf = "";
    fhold;
  }


  
  ws        = (' ' | '\t');
  char      = ([^"\\] | '\n' | ( '\\' (any | '\n') )) $str_a;
  word      = (alpha [^ \t\n:]*) $str_a;
  real      = ([\-+]? $num_a ('0'..'9' digit* '.' digit+) $num_a );
  integer   = ([\-+]? $num_a ('0'..'9' digit*) $num_a );
            
  number    = real | integer;
  string    = ('"' char* '"') | word;
  hash      = '{'? (ws* string ':' >hash_value)* ws* '}'?;
#  array   = '[]' | '[' ($value)* $array_element ']';
  
  main     := ws* (string @string | number @number | hash @hash); # | array | true | false | null;
  
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

std::ostream& operator<< (std::ostream& pStream, const Value& val)
{
  if (val.mPtr && val.mPtr->mDataPtr) {
    val.mPtr->mDataPtr->to_stream(pStream);
  } else {
    pStream << "Nil";
  }
  return pStream;
}


/** Display number inside stream. */
template<>
void MatrixData::to_stream(std::ostream& pStream) const
{
  char buffer[20];
  if (size() == 0) {
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "] 0>";
#else
    pStream << "<" << type_name() << " 0>";
#endif
  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    snprintf(buffer, 20, "<%s[%lu] [ % .2f", type_name(), mId, data[start]);
#else
    snprintf(buffer, 20, "<%s [ % .2f", type_name(), data[start]);
#endif
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
    pStream << "<" << type_name() << "[" << mId << "] 0>";
#else
    pStream << "<" << type_name() << " 0>";
#endif
  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "]" << " [ " << data[start];
#else
    pStream << "<" << type_name() << "[" << mId << "]" << " [ " << data[start];
#endif
    for (size_t i= start+1; i < start+sz; i++) {
      pStream << " " << data[i];
    }
    pStream << " ], " << mRowCount << "x" << mColCount << ">";
  }
}