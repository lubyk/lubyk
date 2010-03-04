
#line 1 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

// include all
#include "rubyk.h"

//#define DEBUG_PARSER

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


#line 45 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
static const char _text_command_actions[] = {
	0, 1, 0, 1, 1, 1, 3, 1, 
	4, 1, 5, 1, 11, 1, 13, 1, 
	14, 1, 15, 2, 0, 1, 2, 2, 
	6, 2, 2, 7, 2, 2, 8, 2, 
	2, 9, 2, 2, 10, 2, 5, 1, 
	2, 11, 13, 2, 12, 13, 3, 0, 
	5, 1, 3, 2, 9, 13, 3, 2, 
	10, 13, 3, 3, 12, 13, 3, 5, 
	0, 1
};

static const short _text_command_key_offsets[] = {
	0, 0, 8, 11, 12, 26, 30, 35, 
	39, 47, 58, 60, 64, 67, 75, 77, 
	77, 79, 86, 90, 93, 95, 101, 101, 
	109, 117, 119, 126, 133, 141, 141, 143, 
	143, 145, 154, 163, 176, 182, 194, 198, 
	209, 220, 232, 233, 239, 251, 255, 266, 
	277, 289, 300, 302, 306, 309, 317, 319, 
	319, 321, 328, 329, 331, 337, 337, 345, 
	353, 355, 362, 369, 377, 377, 379, 379, 
	381, 390, 399, 412, 416, 427, 431, 432, 
	443, 457, 458, 466
};

static const char _text_command_trans_keys[] = {
	9, 10, 32, 35, 46, 47, 97, 122, 
	9, 10, 32, 10, 9, 10, 32, 40, 
	61, 95, 124, 126, 47, 57, 65, 90, 
	97, 122, 9, 32, 61, 124, 9, 32, 
	62, 65, 90, 9, 32, 65, 90, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	39, 41, 43, 45, 46, 47, 48, 57, 
	97, 122, 34, 92, 9, 32, 41, 44, 
	9, 32, 44, 9, 32, 34, 39, 43, 
	45, 48, 57, 39, 92, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 9, 10, 
	32, 35, 9, 32, 35, 48, 57, 9, 
	32, 41, 44, 48, 57, 58, 95, 47, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	39, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 46, 47, 97, 122, 9, 32, 
	41, 46, 47, 97, 122, 58, 95, 46, 
	57, 65, 90, 97, 122, 39, 92, 48, 
	57, 9, 32, 41, 46, 47, 48, 57, 
	97, 122, 47, 58, 95, 48, 57, 65, 
	90, 97, 122, 9, 32, 41, 46, 47, 
	58, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 46, 47, 97, 122, 9, 10, 
	32, 35, 95, 126, 47, 57, 65, 90, 
	97, 122, 46, 47, 97, 122, 9, 10, 
	32, 35, 95, 47, 57, 65, 90, 97, 
	122, 9, 10, 32, 35, 95, 46, 57, 
	65, 90, 97, 122, 9, 10, 32, 35, 
	95, 126, 46, 57, 65, 90, 97, 122, 
	124, 9, 32, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 126, 47, 57, 65, 
	90, 97, 122, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 9, 10, 32, 35, 95, 46, 
	57, 65, 90, 97, 122, 9, 10, 32, 
	35, 95, 126, 46, 57, 65, 90, 97, 
	122, 34, 39, 41, 43, 45, 46, 47, 
	48, 57, 97, 122, 34, 92, 9, 32, 
	41, 44, 9, 32, 44, 9, 32, 34, 
	39, 43, 45, 48, 57, 39, 92, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	10, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 47, 57, 65, 90, 97, 
	122, 9, 32, 34, 39, 43, 45, 48, 
	57, 34, 92, 9, 32, 41, 46, 47, 
	97, 122, 9, 32, 41, 46, 47, 97, 
	122, 58, 95, 46, 57, 65, 90, 97, 
	122, 39, 92, 48, 57, 9, 32, 41, 
	46, 47, 48, 57, 97, 122, 47, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 41, 46, 47, 58, 95, 48, 57, 
	65, 90, 97, 122, 46, 47, 97, 122, 
	9, 32, 61, 95, 124, 47, 57, 65, 
	90, 97, 122, 9, 32, 61, 124, 62, 
	9, 32, 61, 95, 124, 46, 57, 65, 
	90, 97, 122, 9, 10, 32, 40, 61, 
	95, 124, 126, 46, 57, 65, 90, 97, 
	122, 10, 9, 10, 32, 35, 46, 47, 
	97, 122, 0
};

static const char _text_command_single_lengths[] = {
	0, 6, 3, 1, 8, 4, 3, 2, 
	2, 7, 2, 4, 3, 6, 2, 0, 
	0, 5, 4, 3, 0, 4, 0, 2, 
	6, 2, 5, 5, 2, 0, 2, 0, 
	0, 5, 3, 7, 4, 6, 2, 5, 
	5, 6, 1, 4, 6, 2, 5, 5, 
	6, 7, 2, 4, 3, 6, 2, 0, 
	0, 5, 1, 0, 4, 0, 2, 6, 
	2, 5, 5, 2, 0, 2, 0, 0, 
	5, 3, 7, 2, 5, 4, 1, 5, 
	8, 1, 6, 0
};

static const char _text_command_range_lengths[] = {
	0, 1, 0, 0, 3, 0, 1, 1, 
	3, 2, 0, 0, 0, 1, 0, 0, 
	1, 1, 0, 0, 1, 1, 0, 3, 
	1, 0, 1, 1, 3, 0, 0, 0, 
	1, 2, 3, 3, 1, 3, 1, 3, 
	3, 3, 0, 1, 3, 1, 3, 3, 
	3, 2, 0, 0, 0, 1, 0, 0, 
	1, 1, 0, 1, 1, 0, 3, 1, 
	0, 1, 1, 3, 0, 0, 0, 1, 
	2, 3, 3, 1, 3, 0, 0, 3, 
	3, 0, 1, 0
};

static const short _text_command_index_offsets[] = {
	0, 0, 8, 12, 14, 26, 31, 36, 
	40, 46, 56, 59, 64, 68, 76, 79, 
	80, 82, 89, 94, 98, 100, 106, 107, 
	113, 121, 124, 131, 138, 144, 145, 148, 
	149, 151, 159, 166, 177, 183, 193, 197, 
	206, 215, 225, 227, 233, 243, 247, 256, 
	265, 275, 285, 288, 293, 297, 305, 308, 
	309, 311, 318, 320, 322, 328, 329, 335, 
	343, 346, 353, 360, 366, 367, 370, 371, 
	373, 381, 388, 399, 403, 412, 417, 419, 
	428, 440, 442, 450
};

static const char _text_command_indicies[] = {
	1, 2, 1, 3, 4, 5, 4, 0, 
	1, 2, 1, 0, 2, 3, 6, 7, 
	6, 8, 9, 4, 10, 11, 4, 4, 
	4, 0, 12, 12, 13, 14, 0, 15, 
	15, 16, 17, 0, 15, 15, 17, 0, 
	18, 17, 17, 17, 17, 0, 19, 20, 
	21, 22, 22, 23, 24, 25, 23, 0, 
	27, 28, 26, 29, 29, 30, 31, 0, 
	32, 32, 33, 0, 33, 33, 19, 20, 
	22, 22, 25, 0, 27, 35, 34, 34, 
	25, 0, 29, 29, 30, 31, 36, 25, 
	0, 37, 38, 37, 39, 0, 40, 40, 
	3, 0, 41, 0, 29, 29, 30, 31, 
	41, 0, 26, 42, 23, 23, 23, 23, 
	0, 42, 42, 43, 44, 45, 45, 46, 
	0, 48, 49, 47, 50, 50, 30, 51, 
	52, 51, 0, 53, 53, 21, 23, 24, 
	23, 0, 42, 23, 23, 23, 23, 0, 
	47, 48, 55, 54, 54, 46, 0, 50, 
	50, 30, 56, 52, 46, 51, 0, 23, 
	42, 23, 57, 23, 23, 0, 50, 50, 
	30, 51, 58, 42, 23, 57, 23, 59, 
	0, 16, 16, 60, 61, 60, 0, 62, 
	63, 62, 64, 60, 65, 60, 60, 60, 
	0, 66, 67, 66, 0, 62, 63, 62, 
	64, 66, 66, 66, 66, 0, 62, 63, 
	62, 64, 66, 66, 66, 66, 0, 62, 
	63, 62, 64, 60, 65, 60, 60, 60, 
	0, 68, 0, 68, 68, 69, 70, 69, 
	0, 71, 72, 71, 73, 69, 74, 69, 
	69, 69, 0, 75, 76, 75, 0, 71, 
	72, 71, 73, 75, 75, 75, 75, 0, 
	71, 72, 71, 73, 75, 75, 75, 75, 
	0, 71, 72, 71, 73, 69, 74, 69, 
	69, 69, 0, 77, 78, 79, 80, 80, 
	81, 82, 83, 81, 0, 85, 86, 84, 
	87, 87, 88, 89, 0, 90, 90, 91, 
	0, 91, 91, 77, 78, 80, 80, 83, 
	0, 85, 93, 92, 92, 83, 0, 87, 
	87, 88, 89, 94, 83, 0, 95, 0, 
	96, 0, 87, 87, 88, 89, 96, 0, 
	84, 97, 81, 81, 81, 81, 0, 97, 
	97, 98, 99, 100, 100, 101, 0, 103, 
	104, 102, 105, 105, 88, 106, 107, 106, 
	0, 108, 108, 79, 81, 82, 81, 0, 
	97, 81, 81, 81, 81, 0, 102, 103, 
	110, 109, 109, 101, 0, 105, 105, 88, 
	111, 107, 101, 106, 0, 81, 97, 81, 
	112, 81, 81, 0, 105, 105, 88, 106, 
	113, 97, 81, 112, 81, 114, 0, 115, 
	116, 115, 0, 117, 117, 118, 115, 119, 
	115, 115, 115, 0, 120, 120, 121, 14, 
	0, 16, 0, 117, 117, 118, 115, 119, 
	115, 115, 115, 0, 6, 7, 6, 8, 
	9, 4, 10, 11, 4, 4, 4, 0, 
	123, 122, 1, 2, 1, 3, 4, 5, 
	4, 0, 124, 0
};

static const char _text_command_trans_targs[] = {
	0, 2, 82, 3, 4, 80, 5, 82, 
	49, 6, 42, 75, 5, 6, 42, 7, 
	36, 8, 9, 10, 14, 18, 16, 23, 
	28, 17, 10, 11, 22, 12, 18, 13, 
	12, 13, 14, 15, 20, 19, 82, 3, 
	19, 21, 24, 25, 30, 32, 33, 25, 
	26, 29, 27, 23, 28, 27, 30, 31, 
	34, 35, 28, 23, 37, 41, 19, 82, 
	3, 38, 39, 40, 43, 44, 48, 19, 
	82, 3, 45, 46, 47, 50, 54, 58, 
	56, 62, 67, 57, 50, 51, 61, 52, 
	58, 53, 52, 53, 54, 55, 59, 82, 
	60, 63, 64, 69, 71, 72, 64, 65, 
	68, 66, 62, 67, 66, 69, 70, 73, 
	74, 67, 62, 76, 79, 77, 78, 42, 
	77, 78, 81, 83, 0
};

static const char _text_command_trans_actions[] = {
	15, 0, 13, 0, 1, 1, 22, 58, 
	5, 22, 22, 22, 0, 0, 0, 0, 
	0, 1, 7, 3, 3, 0, 19, 19, 
	19, 19, 19, 3, 19, 37, 9, 37, 
	3, 3, 19, 19, 19, 11, 40, 11, 
	0, 19, 3, 3, 3, 19, 19, 19, 
	3, 19, 37, 62, 62, 3, 19, 19, 
	46, 19, 46, 46, 1, 1, 31, 50, 
	31, 28, 1, 1, 0, 1, 1, 34, 
	54, 34, 28, 1, 1, 3, 3, 0, 
	19, 19, 19, 19, 19, 3, 19, 37, 
	9, 37, 3, 3, 19, 19, 19, 43, 
	19, 3, 3, 3, 19, 19, 19, 3, 
	19, 37, 62, 62, 3, 19, 19, 46, 
	19, 46, 46, 1, 1, 25, 25, 25, 
	0, 0, 0, 17, 0
};

static const char _text_command_eof_actions[] = {
	0, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 0, 0, 0
};

static const int text_command_start = 1;
static const int text_command_first_final = 82;

static const int text_command_en_eat_line = 81;
static const int text_command_en_main = 1;


#line 44 "/Users/gaspard/git/rubyk/src/core/text_command.rl"


void TextCommand::initialize() {
  int cs;

  silent_     = false;
  clear();
  
#line 304 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
	{
	cs = text_command_start;
	}

#line 52 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
  current_state_ = cs;
}

void TextCommand::listen() {
  char buffer[1024];
  char *line = buffer; //  when not using readline, use 'buffer' for storage

  if (!silent_) *output_ << PLANET_WELCOME;

  clear();

  thread_ready();

  if (init_script_ != "") {
    lock();
      parse(init_script_.c_str());
      parse("\n");
    unlock();
  }

  while(should_run() && getline(&line, 1023)) {
    lock();
      parse(line);
      parse("\n");
      if (should_run()) saveline(line); // command was not a 'quit'
      freeline(line);
    unlock();
  }
}

void TextCommand::parse(const std::string &string) {
  const char *p  = string.data();     // data pointer
  const char *pe = p + string.size(); // past end
  const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
  int cs = current_state_;            // restore machine state

  DEBUG(printf("parse:\"%s\"\n",string.c_str()));

  
#line 349 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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
	_keys = _text_command_trans_keys + _text_command_key_offsets[cs];
	_trans = _text_command_index_offsets[cs];

	_klen = _text_command_single_lengths[cs];
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

	_klen = _text_command_range_lengths[cs];
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
	_trans = _text_command_indicies[_trans];
	cs = _text_command_trans_targs[_trans];

	if ( _text_command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _text_command_actions + _text_command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 91 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      DEBUG(printf("_%c_",(*p)));
      token_ += (*p); /* append */
    }
	break;
	case 1:
#line 96 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      // FIXME: this is a temporary hack until we sub parse with Value...
      parameter_string_ += (*p);
    }
	break;
	case 2:
#line 101 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(var_);}
	break;
	case 3:
#line 103 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(method_);}
	break;
	case 4:
#line 105 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(class_);}
	break;
	case 5:
#line 107 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(value_);}
	break;
	case 6:
#line 109 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ from_node_     = var_; }
	break;
	case 7:
#line 111 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ from_port_ = var_; }
	break;
	case 8:
#line 113 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ to_port_   = var_; }
	break;
	case 9:
#line 119 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      to_node_   = var_;
      create_link();
    }
	break;
	case 10:
#line 124 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      to_node_   = var_;
      remove_link();
    }
	break;
	case 11:
#line 129 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ create_instance(); }
	break;
	case 12:
#line 135 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ execute_command(); }
	break;
	case 13:
#line 139 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      clear();
    }
	break;
	case 14:
#line 143 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "# Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 81; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 15:
#line 152 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 508 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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
	const char *__acts = _text_command_actions + _text_command_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 14:
#line 143 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "# Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 81; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 535 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
		}
	}
	}

	_out: {}
	}

#line 204 "/Users/gaspard/git/rubyk/src/core/text_command.rl"

//  printf("{%s}\n",p);
  current_state_ = cs;
}

void TextCommand::set_from_token(std::string &string) {
  DEBUG(if (&string == &value_) std::cout << "[val " << token_ << "]" << std::endl);
  DEBUG(if (&string == &var_)   std::cout << "[var " << token_ << "]" << std::endl);
  DEBUG(if (&string == &class_) std::cout << "[cla " << token_ << "]" << std::endl);

  string = token_;
  token_ = "";
}

void TextCommand::create_instance() {
  Value params(Json(parameter_string_.c_str()));
  if (params.is_nil()) {
    params.set_type(HASH_VALUE); // empty hash value
  }

  Value list;
  names_to_urls();

  DEBUG(std::cout << "NEW "<< var_ << " = " << class_ << "(" << params << ")");
  list.push_back(var_);
  list.push_back(params);

  Value res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/new"), list);

  Value links;
  if (res.is_string()) {
    links = root_->call(LINK_URL, gNilValue); // create pending links
  }

  if (res.is_string() && !silent_) {
    print_result(root_->call(INSPECT_URL, res));
    for (size_t i = 0; i < links.size(); ++i) {
      print_result(links[i]);
    }
  } else if (!silent_) {
    print_result(res);
  }
}

void TextCommand::change_link(char op) {
  names_to_urls();

  DEBUG(std::cout << op << " LINK " << from_node_ << "." << from_port_ << "=>" << to_port_ << "." << to_node_ << std::endl);
  ListValue list;

  if (from_port_ == "") {
    list.push_back(std::string(from_node_));
  } else {
    list.push_back(std::string(from_node_).append("/out/").append(from_port_));
  }

  if (op == 'c') {
    list.push_back("=>");
  } else {
    list.push_back("||");
  }

  if (to_port_ == "") {
    list.push_back(std::string(to_node_));
  } else {
    list.push_back(std::string(to_node_).append("/in/").append(to_port_));
  }

  print_result(root_->call(LINK_URL, list));
}

void TextCommand::execute_method() {
  Value res;
  // why doesn't this work ? Value params(Json(parameter_string_));
  Value params = Value(Json(parameter_string_));
  names_to_urls();

  DEBUG(std::cout << "METHOD " << var_ << "." << method_ << "(" << params << ")" << std::endl);

  if (method_ == "set") {
    // TODO: should 'set' live in normal tree space ?
    ObjectHandle target;
    if (root_->get_object_at(var_, &target)) {
      // FIXME: this is not correct: we should make ALL objects thread-safe.
      target->lock();
        res = target->set(params);
      target->unlock();
    } else {
      res = ErrorValue(NOT_FOUND_ERROR, var_);
    }
  } else {
    if (method_ == "b") method_ = "bang";
    var_.append("/").append(method_);
    res = root_->call(var_, params);
  }
  print_result(res);
}

void TextCommand::execute_class_method() {
  Value res;
  Value params = Value(Json(parameter_string_));

  DEBUG(std::cout << "CLASS_METHOD " << std::string(CLASS_URL).append("/").append(class_).append("/").append(method_) << "(" << params << ")" << std::endl);
  res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/").append(method_), params);
  print_result(res);
}

void TextCommand::execute_command() {
  Value res;
  Value params = Value(Json(parameter_string_));

  DEBUG(std::cout << "CMD " << method_ << "(" << params << ")" << std::endl);
  if (method_ == "lib") {
    res = root_->call(LIB_URL, params);
  } else if (method_ == "quit" || method_ == "q") {
    quit();  // Readline won't quit with a SIGTERM (see doc/prototypes/term_readline.cpp) so
             // we have to use quit() instead of kill().

    res = root_->call(QUIT_URL, gNilValue);
  } else {
    names_to_urls();
    res = root_->call(method_, params);
  }
  print_result(res);
}

void TextCommand::clear() {
  token_      = "";
  var_        = "";
  class_      = "";
  parameter_string_ = "";
  from_port_  = "";
  to_port_    = "";
}
