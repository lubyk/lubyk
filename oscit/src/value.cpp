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
      out_stream << "\"Matrix " << matrix_->rows << "x" << matrix_->cols << "\"";
      break;
    case MIDI_VALUE:
      out_stream << "\"MidiMessage " << *midi_message_ << "\"";
      break;
    case LIST_VALUE:
      size_t sz = size();
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

///////////////// ====== JSON PARSER ========= /////////////
#line 265 "src/value.rl"


// transition table

#line 161 "src/value.cpp"
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
	0, 0, 16, 31, 33, 41, 54, 63, 
	65, 66, 66, 68, 68, 72, 72, 74, 
	74, 76, 86, 88, 97, 99, 106, 115, 
	117, 118, 128, 130, 130, 134, 141, 141, 
	146, 151, 156, 164, 176, 187, 189, 196, 
	196, 198, 198, 200, 210, 212, 221, 223, 
	230, 239, 241, 242, 252, 254, 254, 258, 
	265, 265, 266, 267, 268, 275, 275, 282, 
	291, 295, 303
};

static const char _json_trans_keys[] = {
	32, 34, 39, 43, 45, 91, 110, 123, 
	9, 10, 48, 57, 65, 90, 97, 122, 
	32, 34, 39, 43, 45, 91, 123, 9, 
	10, 48, 57, 65, 90, 97, 122, 34, 
	92, 0, 32, 44, 58, 93, 125, 9, 
	10, 0, 32, 34, 39, 44, 93, 125, 
	9, 10, 65, 90, 97, 122, 32, 34, 
	39, 9, 10, 65, 90, 97, 122, 34, 
	92, 58, 39, 92, 32, 58, 9, 10, 
	39, 92, 48, 57, 0, 32, 44, 46, 
	93, 125, 9, 10, 48, 57, 48, 57, 
	0, 32, 44, 93, 125, 9, 10, 48, 
	57, 44, 93, 0, 32, 44, 93, 125, 
	9, 10, 32, 34, 39, 9, 10, 65, 
	90, 97, 122, 34, 92, 58, 32, 34, 
	39, 125, 9, 10, 65, 90, 97, 122, 
	39, 92, 32, 58, 9, 10, 0, 32, 
	44, 93, 125, 9, 10, 32, 58, 117, 
	9, 10, 32, 58, 108, 9, 10, 32, 
	58, 108, 9, 10, 0, 32, 44, 58, 
	93, 125, 9, 10, 32, 34, 39, 43, 
	45, 91, 110, 123, 9, 10, 48, 57, 
	32, 34, 39, 43, 45, 91, 123, 9, 
	10, 48, 57, 34, 92, 0, 32, 44, 
	93, 125, 9, 10, 39, 92, 48, 57, 
	0, 32, 44, 46, 93, 125, 9, 10, 
	48, 57, 48, 57, 0, 32, 44, 93, 
	125, 9, 10, 48, 57, 44, 93, 0, 
	32, 44, 93, 125, 9, 10, 32, 34, 
	39, 9, 10, 65, 90, 97, 122, 34, 
	92, 58, 32, 34, 39, 125, 9, 10, 
	65, 90, 97, 122, 39, 92, 32, 58, 
	9, 10, 0, 32, 44, 93, 125, 9, 
	10, 117, 108, 108, 0, 32, 44, 93, 
	125, 9, 10, 0, 32, 44, 93, 125, 
	9, 10, 32, 34, 39, 9, 10, 65, 
	90, 97, 122, 32, 58, 9, 10, 0, 
	32, 44, 58, 93, 125, 9, 10, 0
};

static const char _json_single_lengths[] = {
	0, 8, 7, 2, 6, 7, 3, 2, 
	1, 0, 2, 0, 2, 0, 2, 0, 
	0, 6, 0, 5, 2, 5, 3, 2, 
	1, 4, 2, 0, 2, 5, 0, 3, 
	3, 3, 6, 8, 7, 2, 5, 0, 
	2, 0, 0, 6, 0, 5, 2, 5, 
	3, 2, 1, 4, 2, 0, 2, 5, 
	0, 1, 1, 1, 5, 0, 5, 3, 
	2, 6, 0
};

static const char _json_range_lengths[] = {
	0, 4, 4, 0, 1, 3, 3, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	1, 2, 1, 2, 0, 1, 3, 0, 
	0, 3, 0, 0, 1, 1, 0, 1, 
	1, 1, 1, 2, 2, 0, 1, 0, 
	0, 0, 1, 2, 1, 2, 0, 1, 
	3, 0, 0, 3, 0, 0, 1, 1, 
	0, 0, 0, 0, 1, 0, 1, 3, 
	1, 1, 0
};

static const short _json_index_offsets[] = {
	0, 0, 13, 25, 28, 36, 47, 54, 
	57, 59, 60, 63, 64, 68, 69, 72, 
	73, 75, 84, 86, 94, 97, 104, 111, 
	114, 116, 124, 127, 128, 132, 139, 140, 
	145, 150, 155, 163, 174, 184, 187, 194, 
	195, 198, 199, 201, 210, 212, 220, 223, 
	230, 237, 240, 242, 250, 253, 254, 258, 
	265, 266, 268, 270, 272, 279, 280, 287, 
	294, 298, 306
};

static const char _json_indicies[] = {
	0, 2, 3, 4, 4, 7, 8, 9, 
	0, 5, 6, 6, 1, 0, 2, 3, 
	4, 4, 7, 9, 0, 5, 6, 6, 
	1, 11, 12, 10, 13, 13, 14, 15, 
	13, 13, 13, 1, 16, 17, 18, 19, 
	16, 16, 16, 17, 6, 6, 1, 20, 
	18, 19, 20, 6, 6, 1, 22, 23, 
	21, 15, 1, 21, 22, 25, 24, 24, 
	1, 15, 1, 6, 10, 11, 27, 26, 
	26, 5, 1, 28, 28, 29, 30, 28, 
	28, 28, 5, 1, 31, 1, 28, 28, 
	29, 28, 28, 28, 31, 1, 7, 32, 
	1, 33, 33, 34, 33, 33, 33, 1, 
	9, 35, 36, 9, 37, 37, 1, 39, 
	40, 38, 41, 1, 9, 35, 36, 42, 
	9, 37, 37, 1, 39, 44, 43, 43, 
	1, 41, 1, 37, 16, 16, 45, 16, 
	16, 16, 1, 38, 1, 15, 46, 1, 
	6, 1, 15, 47, 1, 6, 1, 15, 
	48, 1, 6, 49, 50, 51, 15, 49, 
	49, 50, 6, 52, 53, 54, 55, 55, 
	57, 58, 59, 52, 56, 1, 52, 53, 
	54, 55, 55, 57, 59, 52, 56, 1, 
	61, 62, 60, 63, 63, 63, 63, 63, 
	63, 1, 60, 61, 65, 64, 64, 56, 
	1, 66, 66, 66, 67, 66, 66, 66, 
	56, 1, 68, 1, 66, 66, 66, 66, 
	66, 66, 68, 1, 57, 69, 1, 70, 
	70, 70, 70, 70, 70, 1, 59, 71, 
	72, 59, 73, 73, 1, 75, 76, 74, 
	77, 1, 59, 71, 72, 78, 59, 73, 
	73, 1, 75, 80, 79, 79, 1, 77, 
	1, 73, 81, 81, 81, 81, 81, 81, 
	1, 74, 82, 1, 83, 1, 84, 1, 
	85, 85, 85, 85, 85, 85, 1, 1, 
	86, 86, 87, 86, 86, 86, 1, 20, 
	18, 19, 20, 6, 6, 1, 1, 15, 
	1, 6, 88, 86, 89, 15, 88, 88, 
	86, 6, 1, 0
};

static const char _json_trans_targs[] = {
	2, 0, 3, 14, 16, 17, 12, 20, 
	31, 22, 3, 4, 13, 61, 62, 5, 
	61, 63, 7, 10, 6, 7, 8, 9, 
	10, 11, 14, 15, 61, 62, 18, 19, 
	21, 61, 62, 23, 26, 28, 23, 24, 
	30, 25, 29, 26, 27, 62, 32, 33, 
	34, 64, 61, 65, 36, 37, 40, 42, 
	43, 46, 57, 48, 37, 38, 39, 66, 
	40, 41, 66, 44, 45, 47, 66, 49, 
	52, 54, 49, 50, 56, 51, 55, 52, 
	53, 66, 58, 59, 60, 66, 61, 62, 
	64, 65
};

static const char _json_trans_actions[] = {
	0, 0, 0, 0, 1, 1, 1, 5, 
	1, 0, 1, 0, 0, 14, 47, 3, 
	17, 17, 0, 0, 0, 1, 0, 0, 
	1, 0, 1, 0, 11, 42, 1, 1, 
	7, 9, 34, 0, 0, 1, 1, 0, 
	0, 3, 0, 1, 0, 57, 1, 1, 
	1, 26, 23, 62, 0, 0, 0, 1, 
	1, 5, 0, 0, 1, 0, 0, 14, 
	1, 0, 11, 1, 1, 7, 9, 0, 
	0, 1, 1, 0, 0, 3, 0, 1, 
	0, 17, 0, 0, 0, 23, 20, 30, 
	38, 52
};

static const int json_start = 1;
static const int json_first_final = 61;
static const int json_error = 0;

static const int json_en_main_strict = 35;
static const int json_en_main_lazy = 1;

#line 269 "src/value.rl"

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
  
  
#line 357 "src/value.cpp"
	{
	cs = json_start;
	}
#line 283 "src/value.rl"
  
  if (strict_mode) {
    cs = json_en_main_strict;
  } else {
    cs = json_en_main_lazy;
  }
  
  
#line 370 "src/value.cpp"
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
#line 157 "src/value.rl"
	{
     // append a char to build a std::string
    DEBUG(printf("%c-",(*p)));
    if ((*p))
      str_buf += (*p); /* append */
  }
	break;
	case 1:
#line 164 "src/value.rl"
	{
    // become a RealValue
    tmp_val.set(atof(str_buf.c_str()));
    DEBUG(printf("[number %f/%s/%s\n]", tmp_val.r, str_buf.c_str(), tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 2:
#line 171 "src/value.rl"
	{
    // become a StringValue
    tmp_val.set(str_buf);
    DEBUG(printf("[string %s]\n", tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 3:
#line 178 "src/value.rl"
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
#line 191 "src/value.rl"
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
#line 203 "src/value.rl"
	{
    // we have a value in tmp that should be changed into a list [tmp]
    DEBUG(printf("[%p:lazy_list %s]\n", this, tmp_val.to_json().c_str()));
    push_back(tmp_val);
  }
	break;
	case 6:
#line 209 "src/value.rl"
	{
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }
	break;
	case 7:
#line 216 "src/value.rl"
	{
    if (!is_list()) set_type(LIST_VALUE);
    
    DEBUG(printf("[%p:list %s]\n", this, p));
    // FIXME: how to avoid 'return' by telling parsing to stop ?
    return p - json + 1;
  }
	break;
	case 8:
#line 224 "src/value.rl"
	{
    // become a NilValue
    DEBUG(printf("[nil]\n"));
    tmp_val.set_type(NIL_VALUE);
  }
	break;
	case 9:
#line 230 "src/value.rl"
	{
    DEBUG(printf("[set_from_tmp %s]\n", tmp_val.to_json().c_str()));
    if (!is_list() && !is_hash()) *this = tmp_val;
  }
	break;
#line 542 "src/value.cpp"
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
#line 291 "src/value.rl"
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