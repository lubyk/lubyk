/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#line 1 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
#include "oscit/mapper.h"

#include <stdlib.h> // atof

#include <string>
#include <iostream>

namespace oscit {

//#define DEBUG_PARSER

Mapper::Mapper() : map_(200), reverse_map_(200) {}

Mapper::Mapper(size_t hash_table_size) : map_(hash_table_size), reverse_map_(hash_table_size) {}

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

#line 176 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"



#line 69 "/home/gaspard/git/rubyk/oscit/src/mapper.cpp"
static const char _mapper_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10
};

static const unsigned char _mapper_key_offsets[] = {
	0, 0, 11, 23, 25, 28, 33, 37, 
	44, 46, 53, 57, 64, 66, 73, 77, 
	82, 86, 88, 99, 101, 104, 109, 113, 
	120, 122, 129, 133, 140, 142, 149, 153, 
	154, 156, 162, 164, 170, 170, 180, 182, 
	182, 186, 188, 194, 196, 202, 202, 204, 
	204, 208, 209, 209
};

static const char _mapper_trans_keys[] = {
	32, 34, 35, 39, 47, 9, 10, 65, 
	90, 97, 122, 0, 32, 34, 35, 39, 
	47, 9, 10, 65, 90, 97, 122, 34, 
	92, 32, 9, 10, 32, 35, 91, 9, 
	10, 32, 91, 9, 10, 32, 43, 45, 
	9, 10, 48, 57, 48, 57, 32, 44, 
	46, 9, 10, 48, 57, 32, 44, 9, 
	10, 32, 43, 45, 9, 10, 48, 57, 
	48, 57, 32, 46, 93, 9, 10, 48, 
	57, 32, 93, 9, 10, 32, 35, 45, 
	9, 10, 32, 45, 9, 10, 45, 62, 
	32, 34, 35, 39, 47, 9, 10, 65, 
	90, 97, 122, 34, 92, 32, 9, 10, 
	32, 35, 91, 9, 10, 32, 91, 9, 
	10, 32, 43, 45, 9, 10, 48, 57, 
	48, 57, 32, 44, 46, 9, 10, 48, 
	57, 32, 44, 9, 10, 32, 43, 45, 
	9, 10, 48, 57, 48, 57, 32, 46, 
	93, 9, 10, 48, 57, 32, 93, 9, 
	10, 10, 48, 57, 32, 93, 9, 10, 
	48, 57, 48, 57, 32, 44, 9, 10, 
	48, 57, 32, 34, 39, 47, 9, 10, 
	65, 90, 97, 122, 39, 92, 32, 58, 
	9, 10, 48, 57, 32, 93, 9, 10, 
	48, 57, 48, 57, 32, 44, 9, 10, 
	48, 57, 39, 92, 32, 58, 9, 10, 
	10, 0
};

static const char _mapper_single_lengths[] = {
	0, 5, 6, 2, 1, 3, 2, 3, 
	0, 3, 2, 3, 0, 3, 2, 3, 
	2, 2, 5, 2, 1, 3, 2, 3, 
	0, 3, 2, 3, 0, 3, 2, 1, 
	0, 2, 0, 2, 0, 4, 2, 0, 
	2, 0, 2, 0, 2, 0, 2, 0, 
	2, 1, 0, 0
};

static const char _mapper_range_lengths[] = {
	0, 3, 3, 0, 1, 1, 1, 2, 
	1, 2, 1, 2, 1, 2, 1, 1, 
	1, 0, 3, 0, 1, 1, 1, 2, 
	1, 2, 1, 2, 1, 2, 1, 0, 
	1, 2, 1, 2, 0, 3, 0, 0, 
	1, 1, 2, 1, 2, 0, 0, 0, 
	1, 0, 0, 0
};

static const unsigned char _mapper_index_offsets[] = {
	0, 0, 9, 19, 22, 25, 30, 34, 
	40, 42, 48, 52, 58, 60, 66, 70, 
	75, 79, 82, 91, 94, 97, 102, 106, 
	112, 114, 120, 124, 130, 132, 138, 142, 
	144, 146, 151, 153, 158, 159, 167, 170, 
	171, 175, 177, 182, 184, 189, 190, 193, 
	194, 198, 200, 201
};

static const char _mapper_indicies[] = {
	0, 2, 3, 4, 5, 0, 5, 5, 
	1, 7, 0, 2, 3, 4, 5, 0, 
	5, 5, 6, 9, 10, 8, 11, 11, 
	1, 12, 13, 14, 12, 1, 15, 14, 
	15, 1, 14, 16, 16, 14, 17, 1, 
	17, 1, 18, 19, 20, 18, 17, 1, 
	21, 22, 21, 1, 22, 23, 23, 22, 
	24, 1, 24, 1, 25, 26, 27, 25, 
	24, 1, 28, 29, 28, 1, 29, 30, 
	31, 29, 1, 32, 31, 32, 1, 31, 
	33, 1, 33, 34, 35, 36, 37, 33, 
	37, 37, 1, 39, 40, 38, 41, 41, 
	1, 42, 43, 44, 42, 1, 45, 44, 
	45, 1, 44, 46, 46, 44, 47, 1, 
	47, 1, 48, 49, 50, 48, 47, 1, 
	51, 52, 51, 1, 52, 53, 53, 52, 
	54, 1, 54, 1, 55, 56, 57, 55, 
	54, 1, 58, 59, 58, 1, 60, 1, 
	61, 1, 55, 57, 55, 61, 1, 62, 
	1, 48, 49, 48, 62, 1, 38, 63, 
	34, 36, 37, 63, 37, 37, 1, 39, 
	65, 64, 64, 41, 1, 41, 37, 66, 
	1, 25, 27, 25, 66, 1, 67, 1, 
	18, 19, 18, 67, 1, 8, 9, 69, 
	68, 68, 11, 1, 11, 5, 71, 70, 
	6, 1, 0
};

static const char _mapper_trans_targs[] = {
	2, 0, 3, 2, 46, 48, 0, 50, 
	3, 4, 45, 5, 5, 6, 7, 6, 
	8, 9, 10, 11, 43, 10, 11, 12, 
	13, 14, 41, 15, 14, 15, 16, 17, 
	16, 18, 19, 37, 38, 40, 19, 20, 
	36, 21, 21, 22, 23, 22, 24, 25, 
	26, 27, 34, 26, 27, 28, 29, 30, 
	32, 31, 30, 31, 2, 33, 35, 37, 
	38, 39, 42, 44, 46, 47, 49, 51
};

static const char _mapper_trans_actions[] = {
	0, 0, 0, 19, 0, 1, 17, 0, 
	1, 0, 0, 3, 0, 19, 0, 0, 
	1, 1, 7, 7, 1, 0, 0, 1, 
	1, 9, 1, 9, 0, 0, 19, 0, 
	0, 0, 0, 19, 0, 1, 1, 0, 
	0, 5, 0, 19, 0, 0, 1, 1, 
	11, 11, 1, 0, 0, 1, 1, 13, 
	1, 13, 0, 0, 15, 1, 1, 0, 
	1, 0, 1, 1, 1, 0, 0, 21
};

static const char _mapper_eof_actions[] = {
	0, 0, 17, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0
};

static const int mapper_start = 1;
static const int mapper_first_final = 50;
static const int mapper_error = 0;

static const int mapper_en_eat_line = 49;
static const int mapper_en_main = 1;

#line 179 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"

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

 
#line 236 "/home/gaspard/git/rubyk/oscit/src/mapper.cpp"
	{
	cs = mapper_start;
	}
#line 198 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"

 
#line 243 "/home/gaspard/git/rubyk/oscit/src/mapper.cpp"
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
#line 66 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   #ifdef DEBUG_PARSER
     printf("_%c_",(*p));
   #endif
   str_buf += (*p);
  }
	break;
	case 1:
#line 73 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   source_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_url " << source_url << "]" << std::endl;
   #endif
  }
	break;
	case 2:
#line 81 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   target_url = str_buf;
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_url " << target_url << "]" << std::endl;
   #endif
  }
	break;
	case 3:
#line 89 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   source_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_min " << source_min << "]" << std::endl;
   #endif
  }
	break;
	case 4:
#line 97 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   source_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[source_max " << source_max << "]" << std::endl;
   #endif
  }
	break;
	case 5:
#line 105 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   target_min = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_min " << target_min << "]" << std::endl;
   #endif
  }
	break;
	case 6:
#line 113 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
   target_max = atof(str_buf.c_str());
   str_buf = "";
   #ifdef DEBUG_PARSER
     std::cout <<    "[target_max " << target_max << "]" << std::endl;
   #endif
  }
	break;
	case 7:
#line 121 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
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
#line 136 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
    p--; // move back one char
    char error_buffer[10];
    snprintf(error_buffer, 9, "%s", p);
    std::cerr << "# Syntax error near '" << error_buffer << "'." << std::endl;

    source_url = "";
    source_min = 0.0;
    source_max = 0.0;
    target_url = "";
    target_min = 0.0;
    target_max = 0.0;

    state = 1;
    {cs = 49; goto _again;} // eat the rest of the line and continue parsing
  }
	break;
	case 9:
#line 153 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
    state = cs;
    {cs = 49; goto _again;}
  }
	break;
	case 10:
#line 158 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{ {cs = (state); goto _again;} printf("comment: [%s:%i]\n", p, cs);}
	break;
#line 433 "/home/gaspard/git/rubyk/oscit/src/mapper.cpp"
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
#line 136 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"
	{
    p--; // move back one char
    char error_buffer[10];
    snprintf(error_buffer, 9, "%s", p);
    std::cerr << "# Syntax error near '" << error_buffer << "'." << std::endl;

    source_url = "";
    source_min = 0.0;
    source_max = 0.0;
    target_url = "";
    target_min = 0.0;
    target_max = 0.0;

    state = 1;
    {cs = 49; goto _again;} // eat the rest of the line and continue parsing
  }
	break;
#line 468 "/home/gaspard/git/rubyk/oscit/src/mapper.cpp"
		}
	}
	}

	_out: {}
	}
#line 200 "/home/gaspard/git/rubyk/oscit/src/mapper.rl"

 return true;
}

}  // oscit
