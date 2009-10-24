
#line 1 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
/** Ragel parser definition to parse urls. */
#include "oscit/url.h"
#include <sstream>
#include <stdlib.h> // atoi
#include <string.h> // strlen

// #define DEBUG_PARSER

namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

std::ostream &operator<<(std::ostream &out_stream, const Url &url) {
  out_stream << url.location_ << url.path_;
  return out_stream;
}

void Url::rebuild_full_url() {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  full_url_ = os.str();
}

///////////////// ====== URL PARSER ========= /////////////

#line 91 "/Users/gaspard/git/rubyk/oscit/src/url.rl"


// transition table

#line 38 "/Users/gaspard/git/rubyk/oscit/src/url.cpp"
static const char _url_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	5, 2, 2, 0, 2, 3, 0, 2, 
	4, 0
};

static const char _url_key_offsets[] = {
	0, 0, 11, 13, 14, 21, 32, 32, 
	34, 34, 47, 60, 74, 76, 79, 93, 
	94, 95, 103, 110, 116, 125
};

static const char _url_trans_keys[] = {
	34, 39, 95, 46, 47, 48, 57, 65, 
	90, 97, 122, 34, 92, 47, 95, 46, 
	57, 65, 90, 97, 122, 0, 32, 95, 
	9, 10, 46, 57, 65, 90, 97, 122, 
	39, 92, 0, 32, 46, 47, 95, 9, 
	10, 48, 57, 65, 90, 97, 122, 0, 
	32, 95, 9, 10, 46, 47, 48, 57, 
	65, 90, 97, 122, 0, 32, 46, 47, 
	58, 95, 9, 10, 48, 57, 65, 90, 
	97, 122, 48, 57, 47, 48, 57, 0, 
	32, 46, 47, 58, 95, 9, 10, 48, 
	57, 65, 90, 97, 122, 47, 47, 34, 
	39, 48, 57, 65, 90, 97, 122, 46, 
	48, 57, 65, 90, 97, 122, 48, 57, 
	65, 90, 97, 122, 46, 47, 58, 48, 
	57, 65, 90, 97, 122, 0
};

static const char _url_single_lengths[] = {
	0, 3, 2, 1, 1, 3, 0, 2, 
	0, 5, 3, 6, 0, 1, 6, 1, 
	1, 2, 1, 0, 3, 0
};

static const char _url_range_lengths[] = {
	0, 4, 0, 0, 3, 4, 0, 0, 
	0, 4, 5, 4, 1, 1, 4, 0, 
	0, 3, 3, 3, 3, 0
};

static const char _url_index_offsets[] = {
	0, 0, 8, 11, 13, 18, 26, 27, 
	30, 31, 41, 50, 61, 63, 66, 77, 
	79, 81, 87, 92, 96, 103
};

static const char _url_indicies[] = {
	0, 2, 3, 3, 4, 5, 5, 1, 
	7, 8, 6, 9, 1, 3, 3, 3, 
	3, 1, 10, 10, 3, 10, 3, 3, 
	3, 1, 6, 7, 12, 11, 11, 10, 
	10, 13, 3, 3, 10, 4, 4, 4, 
	1, 10, 10, 3, 10, 3, 14, 14, 
	14, 1, 10, 10, 13, 15, 16, 3, 
	10, 14, 14, 14, 1, 17, 1, 18, 
	17, 1, 10, 10, 13, 3, 19, 3, 
	10, 4, 5, 5, 1, 20, 1, 21, 
	1, 0, 2, 22, 22, 22, 1, 23, 
	22, 22, 22, 1, 24, 24, 24, 1, 
	23, 25, 16, 24, 24, 24, 1, 1, 
	0
};

static const char _url_trans_targs[] = {
	2, 0, 7, 5, 9, 14, 2, 3, 
	6, 4, 21, 7, 8, 10, 11, 5, 
	12, 13, 4, 15, 16, 17, 18, 19, 
	20, 4
};

static const char _url_trans_actions[] = {
	0, 0, 0, 1, 1, 1, 1, 0, 
	0, 12, 7, 1, 0, 1, 1, 9, 
	5, 1, 15, 3, 0, 0, 1, 1, 
	1, 9
};

static const int url_start = 1;
static const int url_first_final = 21;
static const int url_error = 0;

static const int url_en_main = 1;


#line 95 "/Users/gaspard/git/rubyk/oscit/src/url.rl"

/** This is a crude JSON parser. */
void Url::parse(const char *url) {
  std::string str_buf;
  // =============== Ragel job ==============

  int cs;
  const char * p  = url;
  const char * pe = url + strlen(p) + 1;

  
#line 138 "/Users/gaspard/git/rubyk/oscit/src/url.cpp"
	{
	cs = url_start;
	}

#line 106 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
  
#line 145 "/Users/gaspard/git/rubyk/oscit/src/url.cpp"
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
	_keys = _url_trans_keys + _url_key_offsets[cs];
	_trans = _url_index_offsets[cs];

	_klen = _url_single_lengths[cs];
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

	_klen = _url_range_lengths[cs];
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
	_trans = _url_indicies[_trans];
	cs = _url_trans_targs[_trans];

	if ( _url_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _url_actions + _url_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 32 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    DEBUG(printf("%c-",(*p)));
    if ((*p)) str_buf.append(&(*p), 1); /* append */
  }
	break;
	case 1:
#line 37 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    location_.protocol_ = str_buf;
    str_buf = "";
    DEBUG(printf("[protocol %s\n]", location_.protocol_.c_str()));
  }
	break;
	case 2:
#line 43 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    location_.name_ = str_buf;
    location_.reference_by_hostname_ = true;
    str_buf = "";
    DEBUG(printf("[host %s\n]", location_.name_.c_str()));
  }
	break;
	case 3:
#line 50 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    location_.name_ = str_buf;
    location_.reference_by_hostname_ = false;
    str_buf = "";
    DEBUG(printf("[servie \"%s\"\n]", location_.name_.c_str()));
  }
	break;
	case 4:
#line 57 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    location_.port_ = atoi(str_buf.c_str());
    str_buf = "";
    DEBUG(printf("[port %i\n]", location_.port_));
  }
	break;
	case 5:
#line 63 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
	{
    path_ = str_buf;
    str_buf = "";
    DEBUG(printf("[path %s\n]", path_.c_str()));
  }
	break;
#line 268 "/Users/gaspard/git/rubyk/oscit/src/url.cpp"
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

#line 107 "/Users/gaspard/git/rubyk/oscit/src/url.rl"
}

} // oscit
