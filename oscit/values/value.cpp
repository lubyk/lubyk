#line 1 "src/core/values/value.rl"
#include "values.h"

/** Ragel parser definition to create Values from JSON. */
#define MAX_NUM_BUFFER_SIZE 50
// #define DEBUG_PARSER

#line 79 "src/core/values/value.rl"


// transition table

#line 14 "src/core/values/value.cpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 1, 3, 
	2, 1, 4, 2, 1, 5, 2, 3, 
	6, 3, 1, 3, 5, 3, 1, 3, 
	6
};

static const unsigned char _json_key_offsets[] = {
	0, 0, 15, 17, 23, 34, 36, 37, 
	37, 40, 52, 52, 58, 62, 66, 66, 
	68, 76, 78, 85, 93, 105, 113, 117, 
	117, 123, 130, 137, 144, 151, 157, 167, 
	172, 172, 183, 186, 198, 206, 211, 219, 
	231, 239, 247
};

static const char _json_trans_keys[] = {
	32, 34, 43, 45, 47, 66, 123, 9, 
	10, 48, 57, 65, 90, 97, 122, 34, 
	92, 0, 32, 58, 125, 9, 10, 0, 
	32, 34, 47, 125, 9, 10, 65, 90, 
	97, 122, 34, 92, 58, 34, 58, 92, 
	0, 32, 34, 58, 92, 125, 9, 10, 
	65, 90, 97, 122, 32, 34, 58, 92, 
	9, 10, 32, 58, 9, 10, 32, 58, 
	9, 10, 48, 57, 0, 32, 46, 125, 
	9, 10, 48, 57, 48, 57, 0, 32, 
	125, 9, 10, 48, 57, 0, 32, 34, 
	58, 92, 125, 9, 10, 0, 32, 34, 
	58, 92, 125, 9, 10, 65, 90, 97, 
	122, 0, 32, 34, 58, 92, 125, 9, 
	10, 32, 58, 9, 10, 0, 32, 58, 
	125, 9, 10, 0, 32, 58, 97, 125, 
	9, 10, 0, 32, 58, 110, 125, 9, 
	10, 0, 32, 58, 103, 125, 9, 10, 
	0, 32, 33, 58, 125, 9, 10, 0, 
	32, 58, 125, 9, 10, 32, 34, 47, 
	125, 9, 10, 65, 90, 97, 122, 0, 
	32, 125, 9, 10, 0, 32, 34, 47, 
	125, 9, 10, 65, 90, 97, 122, 34, 
	58, 92, 0, 32, 34, 58, 92, 125, 
	9, 10, 65, 90, 97, 122, 0, 32, 
	34, 58, 92, 125, 9, 10, 0, 32, 
	125, 9, 10, 0, 32, 34, 58, 92, 
	125, 9, 10, 0, 32, 34, 58, 92, 
	125, 9, 10, 65, 90, 97, 122, 0, 
	32, 34, 58, 92, 125, 9, 10, 0, 
	32, 34, 58, 92, 125, 9, 10, 0, 
	32, 58, 125, 9, 10, 0
};

static const char _json_single_lengths[] = {
	0, 7, 2, 4, 5, 2, 1, 0, 
	3, 6, 0, 4, 2, 2, 0, 0, 
	4, 0, 3, 6, 6, 6, 2, 0, 
	4, 5, 5, 5, 5, 4, 4, 3, 
	0, 5, 3, 6, 6, 3, 6, 6, 
	6, 6, 4
};

static const char _json_range_lengths[] = {
	0, 4, 0, 1, 3, 0, 0, 0, 
	0, 3, 0, 1, 1, 1, 0, 1, 
	2, 1, 2, 1, 3, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 3, 1, 
	0, 3, 0, 3, 1, 1, 1, 3, 
	1, 1, 1
};

static const short _json_index_offsets[] = {
	0, 0, 12, 15, 21, 30, 33, 35, 
	36, 40, 50, 51, 57, 61, 65, 66, 
	68, 75, 77, 83, 91, 101, 109, 113, 
	114, 120, 127, 134, 141, 148, 154, 162, 
	167, 168, 177, 181, 191, 199, 204, 212, 
	222, 230, 238
};

static const char _json_indicies[] = {
	0, 2, 3, 3, 4, 7, 8, 0, 
	5, 6, 6, 1, 10, 11, 9, 12, 
	12, 13, 12, 12, 1, 14, 15, 16, 
	17, 19, 15, 18, 18, 1, 21, 22, 
	20, 13, 1, 20, 1, 23, 24, 17, 
	25, 26, 16, 23, 24, 28, 26, 27, 
	27, 17, 17, 17, 18, 23, 29, 17, 
	27, 1, 13, 1, 18, 17, 23, 17, 
	27, 9, 5, 1, 30, 30, 31, 30, 
	30, 5, 1, 32, 1, 30, 30, 30, 
	30, 32, 1, 33, 33, 1, 34, 35, 
	33, 33, 4, 36, 37, 16, 34, 35, 
	39, 37, 38, 38, 4, 40, 33, 18, 
	34, 41, 40, 33, 38, 4, 34, 4, 
	38, 4, 42, 12, 13, 42, 12, 6, 
	42, 12, 13, 43, 42, 12, 6, 42, 
	12, 13, 44, 42, 12, 6, 42, 12, 
	13, 45, 42, 12, 6, 42, 12, 46, 
	13, 42, 12, 6, 47, 48, 13, 47, 
	48, 6, 8, 16, 17, 49, 8, 18, 
	18, 1, 14, 14, 14, 14, 1, 1, 
	14, 15, 16, 17, 19, 15, 18, 18, 
	1, 1, 23, 24, 17, 25, 26, 16, 
	23, 24, 28, 26, 27, 27, 17, 25, 
	25, 1, 23, 24, 25, 25, 17, 14, 
	14, 14, 14, 1, 33, 33, 1, 34, 
	35, 33, 33, 4, 36, 37, 16, 34, 
	35, 39, 37, 38, 38, 4, 40, 33, 
	18, 34, 41, 40, 33, 38, 36, 36, 
	1, 34, 35, 36, 36, 4, 42, 12, 
	13, 42, 12, 6, 0
};

static const char _json_trans_targs[] = {
	1, 0, 2, 15, 19, 16, 24, 25, 
	30, 2, 3, 14, 32, 4, 32, 33, 
	5, 8, 12, 37, 5, 6, 7, 9, 
	10, 34, 35, 11, 36, 13, 32, 17, 
	18, 38, 20, 23, 38, 39, 21, 41, 
	40, 22, 42, 26, 27, 28, 29, 42, 
	32, 31
};

static const char _json_trans_actions[] = {
	0, 0, 0, 1, 3, 1, 3, 3, 
	0, 3, 0, 3, 7, 9, 11, 11, 
	0, 3, 3, 11, 3, 0, 3, 16, 
	3, 19, 19, 3, 19, 3, 5, 1, 
	1, 13, 16, 3, 25, 25, 3, 25, 
	13, 3, 13, 3, 3, 3, 3, 29, 
	22, 0
};

static const int json_start = 1;
static const int json_first_final = 32;
static const int json_error = 0;

static const int json_en_main = 1;

#line 83 "src/core/values/value.rl"

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
  
  
#line 172 "src/core/values/value.cpp"
	{
	cs = json_start;
	}
#line 100 "src/core/values/value.rl"
  
#line 178 "src/core/values/value.cpp"
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
#line 10 "src/core/values/value.rl"
	{
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(pStr);
    }
#ifdef DEBUG_PARSER
printf("%c_",(*p));
#endif
    num_buf[num_buf_i] = (*p); /* append */
    num_buf_i++;
  }
	break;
	case 1:
#line 24 "src/core/values/value.rl"
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
#line 33 "src/core/values/value.rl"
	{
    num_buf[num_buf_i+1] = '\0';
    Number(atof(num_buf)).set(*this);
  }
	break;
	case 3:
#line 38 "src/core/values/value.rl"
	{
    String(str_buf).set(*this);
    str_buf = "";
  }
	break;
	case 4:
#line 43 "src/core/values/value.rl"
	{
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.from_string(p);
    tmp_h.set_key(str_buf, tmp_val);
    if (p == pe)
      tmp_h.set(*this);
    
    str_buf = "";
    p--;
  }
	break;
	case 5:
#line 55 "src/core/values/value.rl"
	{
    tmp_h.set(*this);
  }
	break;
	case 6:
#line 59 "src/core/values/value.rl"
	{
    gBangValue.set(*this);
  }
	break;
#line 319 "src/core/values/value.cpp"
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
#line 101 "src/core/values/value.rl"
  
  return p - pStr;
}

std::ostream& operator<< (std::ostream& pStream, const Value val)
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