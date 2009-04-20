#line 1 "src/value.rl"
/** Ragel parser definition to create Values from JSON. */

//#define DEBUG_PARSER

#include "oscit/values.h"
#include "oscit/list.h"
#include <iostream>
#include <sstream>

namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


Value gNilValue('N');
Value gEmptyValue;
Hash  gEmptyHash(1);

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
    case LIST_VALUE:
      size_t sz = val.size();
      out_stream << "[";
      for (size_t i = 0; i < sz; ++i) {
        if (i > 0 && !val[i].is_empty()) out_stream << ", ";
        out_stream << val[i];
      }
      out_stream << "]";
      break;
    case EMPTY_VALUE:
      break; // nothing
    case ANY_VALUE: /* continue */
    case NIL_VALUE: /* continue */
    default:
      out_stream << "null";
  }
  return out_stream;
}

Json Value::to_json() const {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
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

///////////////// ====== JSON PARSER ========= /////////////
#line 213 "src/value.rl"


// transition table

#line 110 "src/value.cpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 3, 1, 4, 1, 
	7, 1, 9, 2, 1, 9, 2, 2, 
	9, 2, 6, 9, 2, 7, 9, 2, 
	8, 9, 3, 0, 8, 9, 3, 4, 
	7, 9, 3, 5, 4, 9, 3, 7, 
	0, 9, 4, 1, 5, 4, 9, 4, 
	2, 5, 4, 9, 4, 4, 7, 0, 
	9, 4, 6, 5, 4, 9, 5, 8, 
	5, 4, 0, 9
};

static const short _json_key_offsets[] = {
	0, 0, 15, 29, 31, 39, 51, 59, 
	61, 62, 62, 66, 66, 68, 78, 80, 
	89, 91, 98, 106, 108, 109, 118, 122, 
	129, 129, 134, 139, 144, 152, 163, 173, 
	175, 182, 182, 184, 194, 196, 205, 207, 
	214, 222, 224, 225, 234, 238, 245, 245, 
	246, 247, 248, 255, 255, 262, 270, 274, 
	282
};

static const char _json_trans_keys[] = {
	32, 34, 43, 45, 91, 110, 123, 9, 
	10, 48, 57, 65, 90, 97, 122, 32, 
	34, 43, 45, 91, 123, 9, 10, 48, 
	57, 65, 90, 97, 122, 34, 92, 0, 
	32, 44, 58, 93, 125, 9, 10, 0, 
	32, 34, 44, 93, 125, 9, 10, 65, 
	90, 97, 122, 32, 34, 9, 10, 65, 
	90, 97, 122, 34, 92, 58, 32, 58, 
	9, 10, 48, 57, 0, 32, 44, 46, 
	93, 125, 9, 10, 48, 57, 48, 57, 
	0, 32, 44, 93, 125, 9, 10, 48, 
	57, 44, 93, 0, 32, 44, 93, 125, 
	9, 10, 32, 34, 9, 10, 65, 90, 
	97, 122, 34, 92, 58, 32, 34, 125, 
	9, 10, 65, 90, 97, 122, 32, 58, 
	9, 10, 0, 32, 44, 93, 125, 9, 
	10, 32, 58, 117, 9, 10, 32, 58, 
	108, 9, 10, 32, 58, 108, 9, 10, 
	0, 32, 44, 58, 93, 125, 9, 10, 
	32, 34, 43, 45, 91, 110, 123, 9, 
	10, 48, 57, 32, 34, 43, 45, 91, 
	123, 9, 10, 48, 57, 34, 92, 0, 
	32, 44, 93, 125, 9, 10, 48, 57, 
	0, 32, 44, 46, 93, 125, 9, 10, 
	48, 57, 48, 57, 0, 32, 44, 93, 
	125, 9, 10, 48, 57, 44, 93, 0, 
	32, 44, 93, 125, 9, 10, 32, 34, 
	9, 10, 65, 90, 97, 122, 34, 92, 
	58, 32, 34, 125, 9, 10, 65, 90, 
	97, 122, 32, 58, 9, 10, 0, 32, 
	44, 93, 125, 9, 10, 117, 108, 108, 
	0, 32, 44, 93, 125, 9, 10, 0, 
	32, 44, 93, 125, 9, 10, 32, 34, 
	9, 10, 65, 90, 97, 122, 32, 58, 
	9, 10, 0, 32, 44, 58, 93, 125, 
	9, 10, 0
};

static const char _json_single_lengths[] = {
	0, 7, 6, 2, 6, 6, 2, 2, 
	1, 0, 2, 0, 0, 6, 0, 5, 
	2, 5, 2, 2, 1, 3, 2, 5, 
	0, 3, 3, 3, 6, 7, 6, 2, 
	5, 0, 0, 6, 0, 5, 2, 5, 
	2, 2, 1, 3, 2, 5, 0, 1, 
	1, 1, 5, 0, 5, 2, 2, 6, 
	0
};

static const char _json_range_lengths[] = {
	0, 4, 4, 0, 1, 3, 3, 0, 
	0, 0, 1, 0, 1, 2, 1, 2, 
	0, 1, 3, 0, 0, 3, 1, 1, 
	0, 1, 1, 1, 1, 2, 2, 0, 
	1, 0, 1, 2, 1, 2, 0, 1, 
	3, 0, 0, 3, 1, 1, 0, 0, 
	0, 0, 1, 0, 1, 3, 1, 1, 
	0
};

static const short _json_index_offsets[] = {
	0, 0, 12, 23, 26, 34, 44, 50, 
	53, 55, 56, 60, 61, 63, 72, 74, 
	82, 85, 92, 98, 101, 103, 110, 114, 
	121, 122, 127, 132, 137, 145, 155, 164, 
	167, 174, 175, 177, 186, 188, 196, 199, 
	206, 212, 215, 217, 224, 228, 235, 236, 
	238, 240, 242, 249, 250, 257, 263, 267, 
	275
};

static const char _json_indicies[] = {
	0, 2, 3, 3, 6, 7, 8, 0, 
	4, 5, 5, 1, 0, 2, 3, 3, 
	6, 8, 0, 4, 5, 5, 1, 10, 
	11, 9, 12, 12, 13, 14, 12, 12, 
	12, 1, 15, 16, 17, 15, 15, 15, 
	16, 5, 5, 1, 18, 17, 18, 5, 
	5, 1, 20, 21, 19, 14, 1, 19, 
	1, 14, 1, 5, 9, 4, 1, 22, 
	22, 23, 24, 22, 22, 22, 4, 1, 
	25, 1, 22, 22, 23, 22, 22, 22, 
	25, 1, 6, 26, 1, 27, 27, 28, 
	27, 27, 27, 1, 8, 29, 8, 30, 
	30, 1, 32, 33, 31, 34, 1, 8, 
	29, 35, 8, 30, 30, 1, 1, 34, 
	1, 30, 15, 15, 36, 15, 15, 15, 
	1, 31, 1, 14, 37, 1, 5, 1, 
	14, 38, 1, 5, 1, 14, 39, 1, 
	5, 40, 41, 42, 14, 40, 40, 41, 
	5, 43, 44, 45, 45, 47, 48, 49, 
	43, 46, 1, 43, 44, 45, 45, 47, 
	49, 43, 46, 1, 51, 52, 50, 53, 
	53, 53, 53, 53, 53, 1, 50, 46, 
	1, 54, 54, 54, 55, 54, 54, 54, 
	46, 1, 56, 1, 54, 54, 54, 54, 
	54, 54, 56, 1, 47, 57, 1, 58, 
	58, 58, 58, 58, 58, 1, 49, 59, 
	49, 60, 60, 1, 62, 63, 61, 64, 
	1, 49, 59, 65, 49, 60, 60, 1, 
	1, 64, 1, 60, 66, 66, 66, 66, 
	66, 66, 1, 61, 67, 1, 68, 1, 
	69, 1, 70, 70, 70, 70, 70, 70, 
	1, 1, 71, 71, 72, 71, 71, 71, 
	1, 18, 17, 18, 5, 5, 1, 1, 
	14, 1, 5, 73, 71, 74, 14, 73, 
	73, 71, 5, 1, 0
};

static const char _json_trans_targs[] = {
	2, 0, 3, 12, 13, 10, 16, 25, 
	18, 3, 4, 11, 51, 52, 5, 51, 
	53, 7, 6, 7, 8, 9, 51, 52, 
	14, 15, 17, 51, 52, 19, 22, 19, 
	20, 24, 21, 23, 52, 26, 27, 28, 
	54, 51, 55, 30, 31, 34, 35, 38, 
	47, 40, 31, 32, 33, 56, 56, 36, 
	37, 39, 56, 41, 44, 41, 42, 46, 
	43, 45, 56, 48, 49, 50, 56, 51, 
	52, 54, 55
};

static const char _json_trans_actions[] = {
	0, 0, 0, 1, 1, 1, 5, 1, 
	0, 1, 0, 0, 14, 47, 3, 17, 
	17, 0, 0, 1, 0, 0, 11, 42, 
	1, 1, 7, 9, 34, 0, 1, 1, 
	0, 0, 3, 0, 57, 1, 1, 1, 
	26, 23, 62, 0, 0, 1, 1, 5, 
	0, 0, 1, 0, 0, 14, 11, 1, 
	1, 7, 9, 0, 1, 1, 0, 0, 
	3, 0, 17, 0, 0, 0, 23, 20, 
	30, 38, 52
};

static const int json_start = 1;
static const int json_first_final = 51;
static const int json_error = 0;

static const int json_en_main_strict = 29;
static const int json_en_main_lazy = 1;

#line 217 "src/value.rl"

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
  
  
#line 292 "src/value.cpp"
	{
	cs = json_start;
	}
#line 231 "src/value.rl"
  
  if (strict_mode) {
    cs = json_en_main_strict;
  } else {
    cs = json_en_main_lazy;
  }
  
  
#line 305 "src/value.cpp"
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
#line 106 "src/value.rl"
	{
     // append a char to build a std::string
    DEBUG(printf("%c-",(*p)));
    if ((*p))
      str_buf += (*p); /* append */
  }
	break;
	case 1:
#line 113 "src/value.rl"
	{
    // become a RealValue
    tmp_val.set(atof(str_buf.c_str()));
    DEBUG(printf("[number %f/%s/%s\n]", tmp_val.r, str_buf.c_str(), tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 2:
#line 120 "src/value.rl"
	{
    // become a StringValue
    tmp_val.set(str_buf);
    DEBUG(printf("[string %s]\n", tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 3:
#line 127 "src/value.rl"
	{
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p);
    set(str_buf, tmp_val);
    p--;
    DEBUG(printf("[hash_value \"%s\":%s]\n", str_buf.c_str(), tmp_val.to_json().c_str()));
    DEBUG(printf("[continue \"%s\"]\n",p));
    
    str_buf = "";
  }
	break;
	case 4:
#line 140 "src/value.rl"
	{
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p, true);
    push_back(tmp_val);
    if (*(p-1) == ',') p--; // hold the ',' separator
    
    DEBUG(printf("[%p:list_value %s ==> %s/%s]\n", this, tmp_val.to_json().c_str(), to_json().c_str(), p));
    p--; // eaten by >list_value sub-action
  }
	break;
	case 5:
#line 152 "src/value.rl"
	{
    // we have a value in tmp that should be changed into a list [tmp]
    DEBUG(printf("[%p:lazy_list %s]\n", this, tmp_val.to_json().c_str()));
    push_back(tmp_val);
  }
	break;
	case 6:
#line 158 "src/value.rl"
	{
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }
	break;
	case 7:
#line 165 "src/value.rl"
	{
    if (!is_list()) set_type(LIST_VALUE);
    
    DEBUG(printf("[%p:list %s]\n", this, p));
    // FIXME: how to avoid 'return' by telling parsing to stop ?
    return p - json + 1;
  }
	break;
	case 8:
#line 173 "src/value.rl"
	{
    // become a NilValue
    DEBUG(printf("[nil]\n"));
    tmp_val.set_type(NIL_VALUE);
  }
	break;
	case 9:
#line 179 "src/value.rl"
	{
    DEBUG(printf("[set_from_tmp %s]\n", tmp_val.to_json().c_str()));
    if (!is_list() && !is_hash()) *this = tmp_val;
  }
	break;
#line 477 "src/value.cpp"
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
#line 239 "src/value.rl"
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