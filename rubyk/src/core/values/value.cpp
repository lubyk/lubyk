#line 1 "src/core/values/value.rl"
#include "values.h"

/** Ragel parser definition to create Values from JSON. */
#define MAX_NUM_BUFFER_SIZE 50

#line 69 "src/core/values/value.rl"


// transition table

#line 13 "src/core/values/value.cpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 3, 1, 
	4, 2, 0, 2, 2, 1, 3, 2, 
	5, 4
};

static const char _json_key_offsets[] = {
	0, 0, 2, 4, 5, 5, 9, 9, 
	11, 13, 26, 27, 35, 35, 38, 40
};

static const char _json_trans_keys[] = {
	34, 92, 34, 92, 58, 32, 58, 9, 
	10, 48, 57, 48, 57, 9, 32, 34, 
	43, 45, 123, 125, 48, 57, 65, 90, 
	97, 122, 58, 9, 32, 34, 125, 65, 
	90, 97, 122, 46, 48, 57, 48, 57, 
	32, 58, 9, 10, 0
};

static const char _json_single_lengths[] = {
	0, 2, 2, 1, 0, 2, 0, 0, 
	0, 7, 1, 4, 0, 1, 0, 2
};

static const char _json_range_lengths[] = {
	0, 0, 0, 0, 0, 1, 0, 1, 
	1, 3, 0, 2, 0, 1, 1, 1
};

static const char _json_index_offsets[] = {
	0, 0, 3, 6, 8, 9, 13, 14, 
	16, 18, 29, 31, 38, 39, 42, 44
};

static const char _json_indicies[] = {
	1, 2, 0, 4, 5, 3, 6, 7, 
	3, 7, 6, 7, 8, 0, 9, 7, 
	10, 7, 11, 11, 12, 13, 13, 15, 
	16, 9, 14, 14, 7, 6, 7, 15, 
	15, 17, 16, 8, 8, 7, 7, 18, 
	9, 7, 10, 7, 7, 6, 7, 14, 
	0
};

static const char _json_trans_targs[] = {
	1, 10, 6, 2, 3, 4, 11, 0, 
	5, 13, 14, 9, 1, 7, 15, 11, 
	12, 2, 8
};

static const char _json_trans_actions[] = {
	3, 5, 3, 3, 0, 3, 15, 0, 
	3, 9, 9, 7, 0, 1, 12, 7, 
	7, 0, 1
};

static const int json_start = 9;
static const int json_first_final = 9;
static const int json_error = 0;

static const int json_en_main = 9;

#line 73 "src/core/values/value.rl"

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
  
  
#line 95 "src/core/values/value.cpp"
	{
	cs = json_start;
	}
#line 90 "src/core/values/value.rl"
  
#line 101 "src/core/values/value.cpp"
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
#line 9 "src/core/values/value.rl"
	{
     // append a char to number buffer
    if (num_buf_i >= MAX_NUM_BUFFER_SIZE) {
      std::cerr << "Buffer overflow !" << std::endl;
      // stop parsing
      return strlen(pStr);
    }
    #ifdef DEBUG_PARSER
      printf("_%c_",(*p));
    #endif
      num_buf[num_buf_i] = (*p); /* append */
    num_buf_i++;     
  }
	break;
	case 1:
#line 23 "src/core/values/value.rl"
	{
     // append a char to build a std::string
#ifdef DEBUG_PARSER
    printf("_%c_",(*p));
#endif
    str_buf.append(&(*p), 1); /* append */
  }
	break;
	case 2:
#line 31 "src/core/values/value.rl"
	{
    num_buf[num_buf_i+1] = '\0';
    Number(atof(num_buf)).set(*this);
  }
	break;
	case 3:
#line 36 "src/core/values/value.rl"
	{
    String(str_buf).set(*this);
    str_buf = "";
  }
	break;
	case 4:
#line 41 "src/core/values/value.rl"
	{
    tmp_h.set(*this);
  }
	break;
	case 5:
#line 45 "src/core/values/value.rl"
	{
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.from_string(p);
    tmp_h.set_key(str_buf, tmp_val);
    str_buf = "";
    p--;
  }
	break;
#line 232 "src/core/values/value.cpp"
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
#line 91 "src/core/values/value.rl"
  
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