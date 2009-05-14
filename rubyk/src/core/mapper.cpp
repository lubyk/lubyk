#line 1 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
#include <string>

#include "mapper.h"

//#define DEBUG_PARSER

Mapper::Mapper() : map_(200), reverse_map_(200) {}
  
Mapper::~Mapper() {
  clear();
}

void Mapper::clear() {
  map_.clear();
  reverse_map_.clear();
}

bool Mapper::set_map(const std::string &source_url, Real source_min, Real source_max,
             const std::string &target_url, Real target_min, Real target_max) {
  if (source_min == source_max) {
    // TODO: record line, better error reporting
    error_ = std::string("Source min and max cannot be the same value !");
    return false;
  }
  
  if (target_min == target_max) {
    // TODO: record line, better error reporting
    error_ = "Target min and max cannot be the same value (used for reverse mapping) !";
    return false;
  }
  
  map_.set(source_url, MapElement(target_url, source_min, source_max, target_min, target_max));
  reverse_map_.set(target_url, MapElement(source_url, target_min, target_max, source_min, source_max));
  return true;
}


bool Mapper::map(const std::string &source, Real value, std::string *target, Real *target_value) {
  const MapElement *res;
  if (!map_.get(source, &res)) return false;
  *target       = res->target_url();
  *target_value = res->scale(value);
  return true;
}

bool Mapper::reverse_map(const std::string &source, Real value, std::string *target, Real *target_value) {
  const MapElement *res;
  if (!reverse_map_.get(source, &res)) return false;
  *target        = res->target_url();
  *target_value  = res->scale(value);
  return true;
}

//// Mapping parser ///////

#line 169 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"



#line 62 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.cpp"
static const char _mapper_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10
};

static const unsigned char _mapper_key_offsets[] = {
	0, 0, 12, 14, 17, 22, 26, 33, 
	35, 42, 46, 53, 55, 62, 66, 71, 
	75, 77, 88, 90, 93, 98, 102, 109, 
	111, 118, 122, 129, 131, 138, 142, 143, 
	145, 151, 153, 159, 159, 169, 171, 171, 
	175, 177, 183, 185, 191, 191, 193, 193, 
	197, 198, 198
};

static const char _mapper_trans_keys[] = {
	0, 32, 34, 35, 39, 47, 9, 10, 
	65, 90, 97, 122, 34, 92, 32, 9, 
	10, 32, 35, 91, 9, 10, 32, 91, 
	9, 10, 32, 43, 45, 9, 10, 48, 
	57, 48, 57, 32, 44, 46, 9, 10, 
	48, 57, 32, 44, 9, 10, 32, 43, 
	45, 9, 10, 48, 57, 48, 57, 32, 
	46, 93, 9, 10, 48, 57, 32, 93, 
	9, 10, 32, 35, 45, 9, 10, 32, 
	45, 9, 10, 45, 62, 32, 34, 35, 
	39, 47, 9, 10, 65, 90, 97, 122, 
	34, 92, 32, 9, 10, 32, 35, 91, 
	9, 10, 32, 91, 9, 10, 32, 43, 
	45, 9, 10, 48, 57, 48, 57, 32, 
	44, 46, 9, 10, 48, 57, 32, 44, 
	9, 10, 32, 43, 45, 9, 10, 48, 
	57, 48, 57, 32, 46, 93, 9, 10, 
	48, 57, 32, 93, 9, 10, 10, 48, 
	57, 32, 93, 9, 10, 48, 57, 48, 
	57, 32, 44, 9, 10, 48, 57, 32, 
	34, 39, 47, 9, 10, 65, 90, 97, 
	122, 39, 92, 32, 58, 9, 10, 48, 
	57, 32, 93, 9, 10, 48, 57, 48, 
	57, 32, 44, 9, 10, 48, 57, 39, 
	92, 32, 58, 9, 10, 10, 0
};

static const char _mapper_single_lengths[] = {
	0, 6, 2, 1, 3, 2, 3, 0, 
	3, 2, 3, 0, 3, 2, 3, 2, 
	2, 5, 2, 1, 3, 2, 3, 0, 
	3, 2, 3, 0, 3, 2, 1, 0, 
	2, 0, 2, 0, 4, 2, 0, 2, 
	0, 2, 0, 2, 0, 2, 0, 2, 
	1, 0, 0
};

static const char _mapper_range_lengths[] = {
	0, 3, 0, 1, 1, 1, 2, 1, 
	2, 1, 2, 1, 2, 1, 1, 1, 
	0, 3, 0, 1, 1, 1, 2, 1, 
	2, 1, 2, 1, 2, 1, 0, 1, 
	2, 1, 2, 0, 3, 0, 0, 1, 
	1, 2, 1, 2, 0, 0, 0, 1, 
	0, 0, 0
};

static const unsigned char _mapper_index_offsets[] = {
	0, 0, 10, 13, 16, 21, 25, 31, 
	33, 39, 43, 49, 51, 57, 61, 66, 
	70, 73, 82, 85, 88, 93, 97, 103, 
	105, 111, 115, 121, 123, 129, 133, 135, 
	137, 142, 144, 149, 150, 158, 161, 162, 
	166, 168, 173, 175, 180, 181, 184, 185, 
	189, 191, 192
};

static const char _mapper_indicies[] = {
	1, 2, 3, 4, 5, 6, 2, 6, 
	6, 0, 8, 9, 7, 10, 10, 11, 
	12, 13, 14, 12, 11, 15, 14, 15, 
	11, 14, 16, 16, 14, 17, 11, 17, 
	11, 18, 19, 20, 18, 17, 11, 21, 
	22, 21, 11, 22, 23, 23, 22, 24, 
	11, 24, 11, 25, 26, 27, 25, 24, 
	11, 28, 29, 28, 11, 29, 30, 31, 
	29, 11, 32, 31, 32, 11, 31, 33, 
	11, 33, 34, 35, 36, 37, 33, 37, 
	37, 11, 39, 40, 38, 41, 41, 11, 
	42, 43, 44, 42, 11, 45, 44, 45, 
	11, 44, 46, 46, 44, 47, 11, 47, 
	11, 48, 49, 50, 48, 47, 11, 51, 
	52, 51, 11, 52, 53, 53, 52, 54, 
	11, 54, 11, 55, 56, 57, 55, 54, 
	11, 58, 59, 58, 11, 60, 11, 61, 
	11, 55, 57, 55, 61, 11, 62, 11, 
	48, 49, 48, 62, 11, 38, 63, 34, 
	36, 37, 63, 37, 37, 11, 39, 65, 
	64, 64, 41, 11, 41, 37, 66, 11, 
	25, 27, 25, 66, 11, 67, 11, 18, 
	19, 18, 67, 11, 7, 8, 69, 68, 
	68, 10, 11, 10, 6, 71, 70, 0, 
	11, 0
};

static const char _mapper_trans_targs[] = {
	0, 49, 1, 2, 1, 45, 47, 2, 
	3, 44, 4, 0, 4, 5, 6, 5, 
	7, 8, 9, 10, 42, 9, 10, 11, 
	12, 13, 40, 14, 13, 14, 15, 16, 
	15, 17, 18, 36, 37, 39, 18, 19, 
	35, 20, 20, 21, 22, 21, 23, 24, 
	25, 26, 33, 25, 26, 27, 28, 29, 
	31, 30, 29, 30, 1, 32, 34, 36, 
	37, 38, 41, 43, 45, 46, 48, 50
};

static const char _mapper_trans_actions[] = {
	17, 0, 0, 0, 19, 0, 1, 1, 
	0, 0, 3, 0, 0, 19, 0, 0, 
	1, 1, 7, 7, 1, 0, 0, 1, 
	1, 9, 1, 9, 0, 0, 19, 0, 
	0, 0, 0, 19, 0, 1, 1, 0, 
	0, 5, 0, 19, 0, 0, 1, 1, 
	11, 11, 1, 0, 0, 1, 1, 13, 
	1, 13, 0, 0, 15, 1, 1, 0, 
	1, 0, 1, 1, 1, 0, 0, 21
};

static const char _mapper_eof_actions[] = {
	0, 17, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int mapper_start = 1;
static const int mapper_first_final = 49;
static const int mapper_error = 0;

static const int mapper_en_eat_line = 48;
static const int mapper_en_main = 1;

#line 172 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"

bool Mapper::parse(const std::string &definitions) {
 std::string script(definitions);
 script.append("\n");
 int cs, state;
 const char * p  = script.data(); // data pointer
 const char * pe = p + script.size(); // past end
 const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
 #ifdef DEBUG_PARSER
   printf("Parsing: \"%s\"", script.c_str());
 #endif

 // source state, target state
 std::string source_url;
 std::string target_url;
 std::string str_buf;
 Real source_min, source_max, target_min, target_max;

 
#line 226 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.cpp"
	{
	cs = mapper_start;
	}
#line 191 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"

 
#line 233 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.cpp"
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
	_keys = _mapper_trans_keys + _mapper_key_offsets[cs];
	_trans = _mapper_index_offsets[cs];

	_klen = _mapper_single_lengths[cs];
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

	_klen = _mapper_range_lengths[cs];
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
	_trans = _mapper_indicies[_trans];
	cs = _mapper_trans_targs[_trans];

	if ( _mapper_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _mapper_actions + _mapper_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 59 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   #ifdef DEBUG_PARSER
     printf("_%c_",(*p));
   #endif
   str_buf += (*p);
  }
	break;
	case 1:
#line 66 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   source_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_url " << source_url << "]" << std::endl;
   #endif
  }
	break;
	case 2:
#line 74 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   target_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_url " << target_url << "]" << std::endl;
   #endif
  }
	break;
	case 3:
#line 82 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   source_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_min " << source_min << "]" << std::endl;
   #endif
  }
	break;
	case 4:
#line 90 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   source_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_max " << source_max << "]" << std::endl;
   #endif
  }
	break;
	case 5:
#line 98 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   target_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_min " << target_min << "]" << std::endl;
   #endif
  }
	break;
	case 6:
#line 106 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   target_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_max " << target_max << "]" << std::endl;
   #endif
  }
	break;
	case 7:
#line 114 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
   #ifdef DEBUG_PARSER
     std::cout << "[set_map " << source_url << " [" << source_min << ", " << source_max << "]" << " --> " <<
                                 target_url << " [" << target_min << ", " << target_max << "]" << std::endl;
   #endif
   if (!set_map(source_url, source_min, source_max, target_url, target_min, target_max)) return false;

   source_url = "";
   source_min = 0.0;
   source_max = 0.0;
   target_url = "";
   target_min = 0.0;
   target_max = 0.0;
  }
	break;
	case 8:
#line 129 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
    p--; // move back one char
    char error_buffer[10];
    snprintf(error_buffer, 9, "%s", p);
    std::cout << "# Syntax error near '" << error_buffer << "'." << std::endl;
    
    source_url = "";
    source_min = 0.0;
    source_max = 0.0;
    target_url = "";
    target_min = 0.0;
    target_max = 0.0;
    
    state = 1;
    {cs = 48; goto _again;} // eat the rest of the line and continue parsing
  }
	break;
	case 9:
#line 146 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
    state = cs;
    {cs = 48; goto _again;}
  }
	break;
	case 10:
#line 151 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{ {cs = (state); goto _again;} printf("comment: [%s:%i]\n", p, cs);}
	break;
#line 423 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _mapper_actions + _mapper_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 8:
#line 129 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"
	{
    p--; // move back one char
    char error_buffer[10];
    snprintf(error_buffer, 9, "%s", p);
    std::cout << "# Syntax error near '" << error_buffer << "'." << std::endl;
    
    source_url = "";
    source_min = 0.0;
    source_max = 0.0;
    target_url = "";
    target_min = 0.0;
    target_max = 0.0;
    
    state = 1;
    {cs = 48; goto _again;} // eat the rest of the line and continue parsing
  }
	break;
#line 458 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.cpp"
		}
	}
	}

	_out: {}
	}
#line 193 "/Users/gaspard/git/rubyk/rubyk/src/core/mapper.rl"

 return true;
}
