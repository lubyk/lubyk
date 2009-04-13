#line 1 "src/value.rl"
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

#line 157 "src/value.rl"


// transition table

#line 60 "src/value.cpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 6, 1, 7, 1, 
	9, 2, 1, 3, 2, 1, 4, 2, 
	1, 6, 2, 3, 8, 2, 5, 9, 
	3, 1, 3, 6, 3, 1, 3, 8
	
};

static const short _json_key_offsets[] = {
	0, 0, 16, 18, 26, 39, 41, 42, 
	42, 45, 59, 59, 65, 69, 73, 73, 
	75, 85, 87, 96, 106, 120, 130, 134, 
	134, 142, 147, 154, 163, 172, 181, 189, 
	199, 206, 206, 219, 222, 236, 246, 253, 
	263, 277, 287, 297
};

static const char _json_trans_keys[] = {
	32, 34, 43, 45, 47, 91, 110, 123, 
	9, 10, 48, 57, 65, 90, 97, 122, 
	34, 92, 0, 32, 44, 58, 93, 125, 
	9, 10, 0, 32, 34, 44, 47, 93, 
	125, 9, 10, 65, 90, 97, 122, 34, 
	92, 58, 34, 58, 92, 0, 32, 34, 
	44, 58, 92, 93, 125, 9, 10, 65, 
	90, 97, 122, 32, 34, 58, 92, 9, 
	10, 32, 58, 9, 10, 32, 58, 9, 
	10, 48, 57, 0, 32, 44, 46, 93, 
	125, 9, 10, 48, 57, 48, 57, 0, 
	32, 44, 93, 125, 9, 10, 48, 57, 
	0, 32, 34, 44, 58, 92, 93, 125, 
	9, 10, 0, 32, 34, 44, 58, 92, 
	93, 125, 9, 10, 65, 90, 97, 122, 
	0, 32, 34, 44, 58, 92, 93, 125, 
	9, 10, 32, 58, 9, 10, 0, 32, 
	44, 58, 93, 125, 9, 10, 32, 44, 
	93, 9, 10, 0, 32, 44, 93, 125, 
	9, 10, 0, 32, 44, 58, 93, 117, 
	125, 9, 10, 0, 32, 44, 58, 93, 
	108, 125, 9, 10, 0, 32, 44, 58, 
	93, 108, 125, 9, 10, 0, 32, 44, 
	58, 93, 125, 9, 10, 32, 34, 47, 
	125, 9, 10, 65, 90, 97, 122, 0, 
	32, 44, 93, 125, 9, 10, 0, 32, 
	34, 44, 47, 93, 125, 9, 10, 65, 
	90, 97, 122, 34, 58, 92, 0, 32, 
	34, 44, 58, 92, 93, 125, 9, 10, 
	65, 90, 97, 122, 0, 32, 34, 44, 
	58, 92, 93, 125, 9, 10, 0, 32, 
	44, 93, 125, 9, 10, 0, 32, 34, 
	44, 58, 92, 93, 125, 9, 10, 0, 
	32, 34, 44, 58, 92, 93, 125, 9, 
	10, 65, 90, 97, 122, 0, 32, 34, 
	44, 58, 92, 93, 125, 9, 10, 0, 
	32, 34, 44, 58, 92, 93, 125, 9, 
	10, 0, 32, 44, 58, 93, 125, 9, 
	10, 0
};

static const char _json_single_lengths[] = {
	0, 8, 2, 6, 7, 2, 1, 0, 
	3, 8, 0, 4, 2, 2, 0, 0, 
	6, 0, 5, 8, 8, 8, 2, 0, 
	6, 3, 5, 7, 7, 7, 6, 4, 
	5, 0, 7, 3, 8, 8, 5, 8, 
	8, 8, 8, 6
};

static const char _json_range_lengths[] = {
	0, 4, 0, 1, 3, 0, 0, 0, 
	0, 3, 0, 1, 1, 1, 0, 1, 
	2, 1, 2, 1, 3, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	1, 0, 3, 0, 3, 1, 1, 1, 
	3, 1, 1, 1
};

static const short _json_index_offsets[] = {
	0, 0, 13, 16, 24, 35, 38, 40, 
	41, 45, 57, 58, 64, 68, 72, 73, 
	75, 84, 86, 94, 104, 116, 126, 130, 
	131, 139, 144, 151, 160, 169, 178, 186, 
	194, 201, 202, 213, 217, 229, 239, 246, 
	256, 268, 278, 288
};

static const char _json_indicies[] = {
	0, 2, 3, 3, 4, 7, 8, 9, 
	0, 5, 6, 6, 1, 11, 12, 10, 
	13, 13, 13, 14, 13, 13, 13, 1, 
	15, 16, 17, 15, 18, 15, 20, 16, 
	19, 19, 1, 22, 23, 21, 14, 1, 
	21, 1, 24, 25, 18, 26, 27, 17, 
	26, 24, 25, 26, 29, 27, 28, 28, 
	18, 18, 18, 19, 24, 30, 18, 28, 
	1, 14, 1, 19, 18, 24, 18, 28, 
	10, 5, 1, 31, 31, 31, 32, 31, 
	31, 31, 5, 1, 33, 1, 31, 31, 
	31, 31, 31, 31, 33, 1, 34, 34, 
	1, 34, 35, 36, 34, 34, 34, 4, 
	37, 38, 17, 37, 35, 36, 37, 40, 
	38, 39, 39, 4, 41, 34, 19, 41, 
	35, 42, 41, 41, 34, 39, 4, 35, 
	4, 39, 4, 43, 13, 43, 14, 43, 
	43, 13, 6, 44, 7, 45, 44, 1, 
	46, 46, 46, 46, 46, 46, 1, 43, 
	13, 43, 14, 43, 47, 43, 13, 6, 
	43, 13, 43, 14, 43, 48, 43, 13, 
	6, 43, 13, 43, 14, 43, 49, 43, 
	13, 6, 50, 51, 50, 14, 50, 50, 
	51, 6, 9, 17, 18, 52, 9, 19, 
	19, 1, 15, 15, 15, 15, 15, 15, 
	1, 1, 15, 16, 17, 15, 18, 15, 
	20, 16, 19, 19, 1, 1, 24, 25, 
	18, 26, 27, 17, 26, 24, 25, 26, 
	29, 27, 28, 28, 18, 26, 26, 1, 
	26, 24, 25, 26, 26, 26, 18, 15, 
	15, 15, 15, 15, 15, 1, 34, 34, 
	1, 34, 35, 36, 34, 34, 34, 4, 
	37, 38, 17, 37, 35, 36, 37, 40, 
	38, 39, 39, 4, 41, 34, 19, 41, 
	35, 42, 41, 41, 34, 39, 37, 37, 
	1, 37, 35, 36, 37, 37, 37, 4, 
	43, 13, 43, 14, 43, 43, 13, 6, 
	0
};

static const char _json_trans_targs[] = {
	1, 0, 2, 15, 19, 16, 24, 25, 
	27, 31, 2, 3, 14, 33, 4, 33, 
	34, 5, 8, 12, 38, 5, 6, 7, 
	9, 10, 35, 36, 11, 37, 13, 33, 
	17, 18, 39, 20, 23, 39, 40, 21, 
	42, 41, 22, 43, 25, 26, 33, 28, 
	29, 30, 43, 33, 32
};

static const char _json_trans_actions[] = {
	0, 0, 0, 1, 3, 1, 3, 29, 
	3, 0, 3, 0, 0, 7, 9, 11, 
	11, 0, 3, 3, 11, 3, 0, 0, 
	20, 0, 23, 23, 3, 23, 3, 5, 
	1, 1, 17, 20, 0, 32, 32, 3, 
	32, 17, 3, 17, 15, 15, 13, 3, 
	3, 3, 36, 26, 0
};

static const int json_start = 1;
static const int json_first_final = 33;
static const int json_error = 0;

static const int json_en_main = 1;

#line 161 "src/value.rl"

/** This is a crude JSON parser. */
size_t Value::build_from_json(const char *json) {
  std::cout << "\nbuild_from_json:\"" << json << "\"\n";
  char num_buf[MAX_NUM_BUFFER_SIZE + 1];
  unsigned int num_buf_i = 0;
  std::string str_buf;
  Value tmp_val;   // used when building Hash or List
  
  // =============== Ragel job ==============
  
  int cs;
  const char * p  = json;
  const char * pe = json + strlen(p) + 1;
  
  
#line 232 "src/value.cpp"
	{
	cs = json_start;
	}
#line 177 "src/value.rl"
  
#line 238 "src/value.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _json_trans_keys + _json_key_offsets[cs];
	_trans = _json_index_offsets[cs];

	_klen = _json_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _json_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _json_indicies[_trans];
	cs = _json_trans_targs[_trans];

	if ( _json_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _json_actions + _json_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 56 "src/value.rl"
	{
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(json);
    }
#ifdef DEBUG_PARSER
printf("%c_",(*p));
#endif
    num_buf[num_buf_i] = (*p); /* append */
    num_buf_i++;
  }
	break;
	case 1:
#line 70 "src/value.rl"
	{
     // append a char to build a std::string
#ifdef DEBUG_PARSER
    printf("%c-",(*p));
#endif
    if ((*p))
      str_buf.append(&(*p), 1); /* append */
  }
	break;
	case 2:
#line 79 "src/value.rl"
	{
    // become a RealValue
    num_buf[num_buf_i+1] = '\0';
    set(atof(num_buf));
  }
	break;
	case 3:
#line 85 "src/value.rl"
	{
    // become a StringValue
    set(str_buf);
    str_buf = "";
  }
	break;
	case 4:
#line 91 "src/value.rl"
	{
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p);
    set(str_buf, tmp_val);
    
    str_buf = "";
    p--;
  }
	break;
	case 5:
#line 102 "src/value.rl"
	{
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p);
    push_back(tmp_val);
    if (*(p-1) == ',') p--; // hold the ',' separator
    
    p--; // eaten by >list_value sub-action
  }
	break;
	case 6:
#line 113 "src/value.rl"
	{
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }
	break;
	case 7:
#line 120 "src/value.rl"
	{
    // become an empty list
    if (!is_list()) {
      set_type(LIST_VALUE);
    }
  }
	break;
	case 8:
#line 127 "src/value.rl"
	{
    // become a NilValue
    set_type(NIL_VALUE);
  }
	break;
	case 9:
#line 132 "src/value.rl"
	{
    printf("%c?",(*p));
  }
	break;
#line 412 "src/value.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}
#line 178 "src/value.rl"
  
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