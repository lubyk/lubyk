#line 1 "src/url.rl"
/** Ragel parser definition to parse urls. */
#include "oscit/url.h"
#include <sstream>
#include <stdlib.h> // atoi
#include <string.h> // strlen

#define MAX_NUM_BUFFER_SIZE 20
//#define DEBUG_PARSER


namespace oscit {

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

std::ostream &operator<<(std::ostream &out_stream, const Url &url) {
  out_stream << url.protocol_ << "://" << url.host_;
  if (url.port_ != 0) {
    out_stream << ":" << url.port_;
  }
  out_stream << url.path_;
  return out_stream;
}

void Url::rebuild_full_url() {
  std::ostringstream os(std::ostringstream::out);
  os << *this;
  full_url_ = os.str();
}

///////////////// ====== URL PARSER ========= /////////////
#line 97 "src/url.rl"


// transition table

#line 42 "src/url.cpp"
static const char _url_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	4, 2, 2, 0, 2, 3, 0
};

static const char _url_key_offsets[] = {
	0, 0, 6, 16, 27, 28, 29, 36, 
	44, 50, 52, 55
};

static const char _url_trans_keys[] = {
	46, 57, 65, 90, 97, 122, 0, 32, 
	9, 10, 46, 57, 65, 90, 97, 122, 
	0, 32, 58, 9, 10, 46, 57, 65, 
	90, 97, 122, 47, 47, 46, 48, 57, 
	65, 90, 97, 122, 47, 58, 46, 57, 
	65, 90, 97, 122, 46, 57, 65, 90, 
	97, 122, 48, 57, 47, 48, 57, 0
};

static const char _url_single_lengths[] = {
	0, 0, 2, 3, 1, 1, 1, 2, 
	0, 0, 1, 0
};

static const char _url_range_lengths[] = {
	0, 3, 4, 4, 0, 0, 3, 3, 
	3, 1, 1, 0
};

static const char _url_index_offsets[] = {
	0, 0, 4, 11, 19, 21, 23, 28, 
	34, 38, 40, 43
};

static const char _url_indicies[] = {
	0, 2, 2, 1, 3, 3, 3, 0, 
	0, 0, 1, 3, 3, 4, 3, 0, 
	2, 2, 1, 5, 1, 6, 1, 7, 
	7, 7, 7, 1, 8, 9, 7, 7, 
	7, 1, 0, 0, 0, 1, 10, 1, 
	11, 10, 1, 1, 0
};

static const char _url_trans_targs[] = {
	2, 0, 3, 11, 4, 5, 6, 7, 
	8, 9, 10, 8
};

static const char _url_trans_actions[] = {
	1, 0, 1, 7, 3, 0, 0, 1, 
	9, 5, 1, 12
};

static const int url_start = 1;
static const int url_first_final = 11;
static const int url_error = 0;

static const int url_en_main = 1;

#line 101 "src/url.rl"

/** This is a crude JSON parser. */
void Url::parse(const char *url) {
  std::string str_buf;
  // =============== Ragel job ==============
  
  int cs;
  const char * p  = url;
  const char * pe = url + strlen(p) + 1;
  
  
#line 115 "src/url.cpp"
	{
	cs = url_start;
	}
#line 112 "src/url.rl"
  
#line 121 "src/url.cpp"
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
#line 50 "src/url.rl"
	{
    DEBUG(printf("%c-",(*p)));
    if ((*p)) str_buf.append(&(*p), 1); /* append */
  }
	break;
	case 1:
#line 55 "src/url.rl"
	{
    protocol_ = str_buf;
    str_buf = "";
    DEBUG(printf("[protocol %s\n]", protocol_.c_str()));
  }
	break;
	case 2:
#line 61 "src/url.rl"
	{
    host_ = str_buf;
    str_buf = "";
    DEBUG(printf("[host %s\n]", host_.c_str()));
  }
	break;
	case 3:
#line 67 "src/url.rl"
	{
    port_ = atoi(str_buf.c_str());
    str_buf = "";
    DEBUG(printf("[port %i\n]", port_));
  }
	break;
	case 4:
#line 73 "src/url.rl"
	{
    path_ = str_buf;
    str_buf = "";
    DEBUG(printf("[path %s\n]", path_.c_str()));
  }
	break;
#line 234 "src/url.cpp"
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
#line 113 "src/url.rl"
}

} // oscit
