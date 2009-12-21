
#line 1 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
#include "oscit/values.h"

//#define DEBUG_PARSER

#include <string.h>  // strlen
#include <stdlib.h>  // atof

#include <iostream>
#include <sstream>


/** Ragel parser definition to create Values from JSON. */
namespace oscit {

//#define DEBUG_PARSER

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

#line 310 "/Users/gaspard/git/rubyk/oscit/src/value.rl"


// transition table

#line 208 "/Users/gaspard/git/rubyk/oscit/src/value.cpp"
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
	0, 0, 16, 31, 33, 41, 58, 71, 
	73, 74, 74, 76, 76, 78, 81, 85, 
	85, 87, 87, 89, 100, 102, 111, 113, 
	120, 133, 135, 136, 151, 153, 153, 155, 
	158, 162, 169, 169, 174, 179, 184, 192, 
	204, 215, 217, 224, 224, 226, 226, 228, 
	238, 240, 249, 251, 258, 271, 273, 274, 
	289, 291, 291, 293, 296, 300, 307, 307, 
	308, 309, 310, 317, 317, 324, 337, 341, 
	349
};

static const char _json_trans_keys[] = {
	32, 34, 39, 43, 45, 91, 110, 123, 
	9, 10, 48, 57, 65, 90, 97, 122, 
	32, 34, 39, 43, 45, 91, 123, 9, 
	10, 48, 57, 65, 90, 97, 122, 34, 
	92, 0, 32, 44, 58, 93, 125, 9, 
	10, 0, 32, 34, 39, 44, 93, 125, 
	9, 10, 43, 45, 48, 57, 65, 90, 
	97, 122, 32, 34, 39, 43, 45, 9, 
	10, 48, 57, 65, 90, 97, 122, 34, 
	92, 58, 39, 92, 48, 57, 58, 48, 
	57, 32, 58, 9, 10, 39, 92, 48, 
	57, 0, 32, 44, 46, 58, 93, 125, 
	9, 10, 48, 57, 48, 57, 0, 32, 
	44, 93, 125, 9, 10, 48, 57, 44, 
	93, 0, 32, 44, 93, 125, 9, 10, 
	32, 34, 39, 43, 45, 9, 10, 48, 
	57, 65, 90, 97, 122, 34, 92, 58, 
	32, 34, 39, 44, 125, 9, 10, 43, 
	45, 48, 57, 65, 90, 97, 122, 39, 
	92, 48, 57, 58, 48, 57, 32, 58, 
	9, 10, 0, 32, 44, 93, 125, 9, 
	10, 32, 58, 117, 9, 10, 32, 58, 
	108, 9, 10, 32, 58, 108, 9, 10, 
	0, 32, 44, 58, 93, 125, 9, 10, 
	32, 34, 39, 43, 45, 91, 110, 123, 
	9, 10, 48, 57, 32, 34, 39, 43, 
	45, 91, 123, 9, 10, 48, 57, 34, 
	92, 0, 32, 44, 93, 125, 9, 10, 
	39, 92, 48, 57, 0, 32, 44, 46, 
	93, 125, 9, 10, 48, 57, 48, 57, 
	0, 32, 44, 93, 125, 9, 10, 48, 
	57, 44, 93, 0, 32, 44, 93, 125, 
	9, 10, 32, 34, 39, 43, 45, 9, 
	10, 48, 57, 65, 90, 97, 122, 34, 
	92, 58, 32, 34, 39, 44, 125, 9, 
	10, 43, 45, 48, 57, 65, 90, 97, 
	122, 39, 92, 48, 57, 58, 48, 57, 
	32, 58, 9, 10, 0, 32, 44, 93, 
	125, 9, 10, 117, 108, 108, 0, 32, 
	44, 93, 125, 9, 10, 0, 32, 44, 
	93, 125, 9, 10, 32, 34, 39, 43, 
	45, 9, 10, 48, 57, 65, 90, 97, 
	122, 32, 58, 9, 10, 0, 32, 44, 
	58, 93, 125, 9, 10, 0
};

static const char _json_single_lengths[] = {
	0, 8, 7, 2, 6, 7, 5, 2, 
	1, 0, 2, 0, 0, 1, 2, 0, 
	2, 0, 0, 7, 0, 5, 2, 5, 
	5, 2, 1, 5, 2, 0, 0, 1, 
	2, 5, 0, 3, 3, 3, 6, 8, 
	7, 2, 5, 0, 2, 0, 0, 6, 
	0, 5, 2, 5, 5, 2, 1, 5, 
	2, 0, 0, 1, 2, 5, 0, 1, 
	1, 1, 5, 0, 5, 5, 2, 6, 
	0
};

static const char _json_range_lengths[] = {
	0, 4, 4, 0, 1, 5, 4, 0, 
	0, 0, 0, 0, 1, 1, 1, 0, 
	0, 0, 1, 2, 1, 2, 0, 1, 
	4, 0, 0, 5, 0, 0, 1, 1, 
	1, 1, 0, 1, 1, 1, 1, 2, 
	2, 0, 1, 0, 0, 0, 1, 2, 
	1, 2, 0, 1, 4, 0, 0, 5, 
	0, 0, 1, 1, 1, 1, 0, 0, 
	0, 0, 1, 0, 1, 4, 1, 1, 
	0
};

static const short _json_index_offsets[] = {
	0, 0, 13, 25, 28, 36, 49, 59, 
	62, 64, 65, 68, 69, 71, 74, 78, 
	79, 82, 83, 85, 95, 97, 105, 108, 
	115, 125, 128, 130, 141, 144, 145, 147, 
	150, 154, 161, 162, 167, 172, 177, 185, 
	196, 206, 209, 216, 217, 220, 221, 223, 
	232, 234, 242, 245, 252, 262, 265, 267, 
	278, 281, 282, 284, 287, 291, 298, 299, 
	301, 303, 305, 312, 313, 320, 330, 334, 
	342
};

static const char _json_indicies[] = {
	0, 2, 3, 4, 4, 7, 8, 9, 
	0, 5, 6, 6, 1, 0, 2, 3, 
	4, 4, 7, 9, 0, 5, 6, 6, 
	1, 11, 12, 10, 13, 13, 14, 15, 
	13, 13, 13, 1, 16, 17, 18, 19, 
	17, 16, 16, 17, 20, 21, 6, 6, 
	1, 22, 18, 19, 20, 20, 22, 21, 
	6, 6, 1, 24, 25, 23, 15, 1, 
	23, 24, 27, 26, 26, 21, 1, 15, 
	21, 1, 1, 15, 1, 6, 10, 11, 
	29, 28, 28, 5, 1, 30, 30, 31, 
	32, 15, 30, 30, 30, 5, 1, 33, 
	1, 30, 30, 31, 30, 30, 30, 33, 
	1, 7, 34, 1, 35, 35, 36, 35, 
	35, 35, 1, 9, 37, 38, 39, 39, 
	9, 40, 41, 41, 1, 43, 44, 42, 
	45, 1, 9, 37, 38, 9, 46, 9, 
	39, 40, 41, 41, 1, 43, 48, 47, 
	47, 40, 1, 45, 40, 1, 1, 45, 
	1, 41, 16, 16, 49, 16, 16, 16, 
	1, 42, 1, 15, 50, 1, 6, 1, 
	15, 51, 1, 6, 1, 15, 52, 1, 
	6, 53, 54, 55, 15, 53, 53, 54, 
	6, 56, 57, 58, 59, 59, 61, 62, 
	63, 56, 60, 1, 56, 57, 58, 59, 
	59, 61, 63, 56, 60, 1, 65, 66, 
	64, 67, 67, 67, 67, 67, 67, 1, 
	64, 65, 69, 68, 68, 60, 1, 70, 
	70, 70, 71, 70, 70, 70, 60, 1, 
	72, 1, 70, 70, 70, 70, 70, 70, 
	72, 1, 61, 73, 1, 74, 74, 74, 
	74, 74, 74, 1, 63, 75, 76, 77, 
	77, 63, 78, 79, 79, 1, 81, 82, 
	80, 83, 1, 63, 75, 76, 63, 84, 
	63, 77, 78, 79, 79, 1, 81, 86, 
	85, 85, 78, 1, 83, 78, 1, 1, 
	83, 1, 79, 87, 87, 87, 87, 87, 
	87, 1, 80, 88, 1, 89, 1, 90, 
	1, 91, 91, 91, 91, 91, 91, 1, 
	1, 92, 92, 93, 92, 92, 92, 1, 
	22, 18, 19, 20, 20, 22, 21, 6, 
	6, 1, 1, 15, 1, 6, 94, 92, 
	95, 15, 94, 94, 92, 6, 1, 0
};

static const char _json_trans_targs[] = {
	2, 0, 3, 16, 18, 19, 14, 22, 
	35, 24, 3, 4, 15, 67, 68, 5, 
	67, 69, 7, 10, 12, 13, 6, 7, 
	8, 9, 10, 11, 16, 17, 67, 68, 
	20, 21, 23, 67, 68, 25, 28, 30, 
	31, 32, 25, 26, 34, 27, 33, 28, 
	29, 68, 36, 37, 38, 70, 67, 71, 
	40, 41, 44, 46, 47, 50, 63, 52, 
	41, 42, 43, 72, 44, 45, 72, 48, 
	49, 51, 72, 53, 56, 58, 59, 60, 
	53, 54, 62, 55, 61, 56, 57, 72, 
	64, 65, 66, 72, 67, 68, 70, 71
};

static const char _json_trans_actions[] = {
	0, 0, 0, 0, 1, 1, 1, 5, 
	1, 0, 1, 0, 0, 14, 47, 3, 
	17, 17, 0, 0, 1, 1, 0, 1, 
	0, 0, 1, 0, 1, 0, 11, 42, 
	1, 1, 7, 9, 34, 0, 0, 1, 
	1, 1, 1, 0, 0, 3, 0, 1, 
	0, 57, 1, 1, 1, 26, 23, 62, 
	0, 0, 0, 1, 1, 5, 0, 0, 
	1, 0, 0, 14, 1, 0, 11, 1, 
	1, 7, 9, 0, 0, 1, 1, 1, 
	1, 0, 0, 3, 0, 1, 0, 17, 
	0, 0, 0, 23, 20, 30, 38, 52
};

static const int json_start = 1;
static const int json_first_final = 67;
static const int json_error = 0;

static const int json_en_main_strict = 39;
static const int json_en_main_lazy = 1;


#line 314 "/Users/gaspard/git/rubyk/oscit/src/value.rl"

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

  
#line 419 "/Users/gaspard/git/rubyk/oscit/src/value.cpp"
	{
	cs = json_start;
	}

#line 328 "/Users/gaspard/git/rubyk/oscit/src/value.rl"

  if (strict_mode) {
    cs = json_en_main_strict;
  } else {
    cs = json_en_main_lazy;
  }

  
#line 433 "/Users/gaspard/git/rubyk/oscit/src/value.cpp"
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
#line 202 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
     // append a char to build a std::string
    DEBUG(printf("%c-",(*p)));
    if ((*p))
      str_buf += (*p); /* append */
  }
	break;
	case 1:
#line 209 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // become a RealValue
    tmp_val.set(atof(str_buf.c_str()));
    DEBUG(printf("[number %f/%s/%s\n]", tmp_val.r, str_buf.c_str(), tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 2:
#line 216 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // become a StringValue
    tmp_val.set(str_buf);
    DEBUG(printf("[string %s]\n", tmp_val.to_json().c_str()));
    str_buf = "";
  }
	break;
	case 3:
#line 223 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // Parse a single element of a hash (key:value)
    // Build tmp_val from string and move p forward
    p++;
    p += tmp_val.build_from_json(p, true);
    set(str_buf, tmp_val);
    p--;
    DEBUG(printf("[hash_value \"%s\":%s]\n", str_buf.c_str(), tmp_val.to_json().c_str()));
    DEBUG(printf("[continue \"%s\"]\n",p));

    str_buf = "";
  }
	break;
	case 4:
#line 236 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
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
#line 248 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // we have a value in tmp that should be changed into a list [tmp]
    DEBUG(printf("[%p:lazy_list %s]\n", this, tmp_val.to_json().c_str()));
    push_back(tmp_val);
  }
	break;
	case 6:
#line 254 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // become an empty HashValue
    if (!is_hash()) {
      set_type(HASH_VALUE);
    }
  }
	break;
	case 7:
#line 261 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    if (!is_list()) set_type(LIST_VALUE);

    DEBUG(printf("[%p:list %s]\n", this, p));
    // FIXME: how to avoid 'return' by telling parsing to stop ?
    return p - json + 1;
  }
	break;
	case 8:
#line 269 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    // become a NilValue
    DEBUG(printf("[nil]\n"));
    tmp_val.set_type(NIL_VALUE);
  }
	break;
	case 9:
#line 275 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
	{
    DEBUG(printf("[set_from_tmp %s]\n", tmp_val.to_json().c_str()));
    if (!is_list() && !is_hash()) *this = tmp_val;
  }
	break;
#line 605 "/Users/gaspard/git/rubyk/oscit/src/value.cpp"
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

#line 336 "/Users/gaspard/git/rubyk/oscit/src/value.rl"
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
