#line 1 "src/core/text_command.rl"
#include "text_command.h"
#include "rubyk.h"

//#define DEBUG_PARSER

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


#line 15 "src/core/text_command.cpp"
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
	39, 47, 57, 59, 63, 66, 73, 75, 
	82, 86, 89, 91, 97, 97, 105, 112, 
	114, 121, 128, 136, 136, 138, 147, 156, 
	169, 175, 187, 191, 202, 213, 225, 226, 
	232, 244, 248, 259, 270, 282, 292, 294, 
	298, 301, 308, 310, 317, 318, 320, 326, 
	326, 334, 341, 343, 350, 357, 365, 365, 
	367, 376, 385, 398, 402, 413, 417, 418, 
	429, 443, 444, 452
};

static const char _text_command_trans_keys[] = {
	9, 10, 32, 35, 46, 47, 97, 122, 
	9, 10, 32, 10, 9, 10, 32, 40, 
	61, 95, 124, 126, 47, 57, 65, 90, 
	97, 122, 9, 32, 61, 124, 9, 32, 
	62, 65, 90, 9, 32, 65, 90, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	41, 43, 45, 46, 47, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 43, 45, 48, 
	57, 48, 57, 9, 32, 41, 44, 46, 
	48, 57, 9, 10, 32, 35, 9, 32, 
	35, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 47, 57, 65, 90, 97, 
	122, 9, 32, 34, 43, 45, 48, 57, 
	34, 92, 9, 32, 41, 46, 47, 97, 
	122, 9, 32, 41, 46, 47, 97, 122, 
	58, 95, 46, 57, 65, 90, 97, 122, 
	48, 57, 9, 32, 41, 46, 47, 48, 
	57, 97, 122, 47, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 41, 46, 
	47, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 126, 47, 57, 65, 
	90, 97, 122, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 9, 10, 32, 35, 95, 46, 
	57, 65, 90, 97, 122, 9, 10, 32, 
	35, 95, 126, 46, 57, 65, 90, 97, 
	122, 124, 9, 32, 46, 47, 97, 122, 
	9, 10, 32, 35, 95, 126, 47, 57, 
	65, 90, 97, 122, 46, 47, 97, 122, 
	9, 10, 32, 35, 95, 47, 57, 65, 
	90, 97, 122, 9, 10, 32, 35, 95, 
	46, 57, 65, 90, 97, 122, 9, 10, 
	32, 35, 95, 126, 46, 57, 65, 90, 
	97, 122, 34, 41, 43, 45, 46, 47, 
	48, 57, 97, 122, 34, 92, 9, 32, 
	41, 44, 9, 32, 44, 9, 32, 34, 
	43, 45, 48, 57, 48, 57, 9, 32, 
	41, 44, 46, 48, 57, 10, 48, 57, 
	9, 32, 41, 44, 48, 57, 58, 95, 
	47, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 46, 47, 97, 122, 9, 32, 
	41, 46, 47, 97, 122, 58, 95, 46, 
	57, 65, 90, 97, 122, 48, 57, 9, 
	32, 41, 46, 47, 48, 57, 97, 122, 
	47, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 41, 46, 47, 58, 95, 
	48, 57, 65, 90, 97, 122, 46, 47, 
	97, 122, 9, 32, 61, 95, 124, 47, 
	57, 65, 90, 97, 122, 9, 32, 61, 
	124, 62, 9, 32, 61, 95, 124, 46, 
	57, 65, 90, 97, 122, 9, 10, 32, 
	40, 61, 95, 124, 126, 46, 57, 65, 
	90, 97, 122, 10, 9, 10, 32, 35, 
	46, 47, 97, 122, 0
};

static const char _text_command_single_lengths[] = {
	0, 6, 3, 1, 8, 4, 3, 2, 
	2, 6, 2, 4, 3, 5, 0, 5, 
	4, 3, 0, 4, 0, 2, 5, 2, 
	5, 5, 2, 0, 0, 5, 3, 7, 
	4, 6, 2, 5, 5, 6, 1, 4, 
	6, 2, 5, 5, 6, 6, 2, 4, 
	3, 5, 0, 5, 1, 0, 4, 0, 
	2, 5, 2, 5, 5, 2, 0, 0, 
	5, 3, 7, 2, 5, 4, 1, 5, 
	8, 1, 6, 0
};

static const char _text_command_range_lengths[] = {
	0, 1, 0, 0, 3, 0, 1, 1, 
	3, 2, 0, 0, 0, 1, 1, 1, 
	0, 0, 1, 1, 0, 3, 1, 0, 
	1, 1, 3, 0, 1, 2, 3, 3, 
	1, 3, 1, 3, 3, 3, 0, 1, 
	3, 1, 3, 3, 3, 2, 0, 0, 
	0, 1, 1, 1, 0, 1, 1, 0, 
	3, 1, 0, 1, 1, 3, 0, 1, 
	2, 3, 3, 1, 3, 0, 0, 3, 
	3, 0, 1, 0
};

static const short _text_command_index_offsets[] = {
	0, 0, 8, 12, 14, 26, 31, 36, 
	40, 46, 55, 58, 63, 67, 74, 76, 
	83, 88, 92, 94, 100, 101, 107, 114, 
	117, 124, 131, 137, 138, 140, 148, 155, 
	166, 172, 182, 186, 195, 204, 214, 216, 
	222, 232, 236, 245, 254, 264, 273, 276, 
	281, 285, 292, 294, 301, 303, 305, 311, 
	312, 318, 325, 328, 335, 342, 348, 349, 
	351, 359, 366, 377, 381, 390, 395, 397, 
	406, 418, 420, 428
};

static const char _text_command_indicies[] = {
	1, 2, 1, 3, 4, 5, 4, 0, 
	1, 2, 1, 0, 2, 3, 6, 7, 
	6, 8, 9, 4, 10, 11, 4, 4, 
	4, 0, 12, 12, 13, 14, 0, 15, 
	15, 16, 17, 0, 15, 15, 17, 0, 
	18, 17, 17, 17, 17, 0, 19, 20, 
	21, 21, 22, 23, 24, 22, 0, 26, 
	27, 25, 28, 28, 29, 30, 0, 31, 
	31, 32, 0, 32, 32, 19, 21, 21, 
	24, 0, 24, 0, 28, 28, 29, 30, 
	33, 24, 0, 34, 35, 34, 36, 0, 
	37, 37, 3, 0, 38, 0, 28, 28, 
	29, 30, 38, 0, 25, 39, 22, 22, 
	22, 22, 0, 39, 39, 40, 41, 41, 
	42, 0, 44, 45, 43, 46, 46, 29, 
	47, 48, 47, 0, 49, 49, 20, 22, 
	23, 22, 0, 39, 22, 22, 22, 22, 
	0, 43, 42, 0, 46, 46, 29, 50, 
	48, 42, 47, 0, 22, 39, 22, 51, 
	22, 22, 0, 46, 46, 29, 47, 52, 
	39, 22, 51, 22, 53, 0, 16, 16, 
	54, 55, 54, 0, 56, 57, 56, 58, 
	54, 59, 54, 54, 54, 0, 60, 61, 
	60, 0, 56, 57, 56, 58, 60, 60, 
	60, 60, 0, 56, 57, 56, 58, 60, 
	60, 60, 60, 0, 56, 57, 56, 58, 
	54, 59, 54, 54, 54, 0, 62, 0, 
	62, 62, 63, 64, 63, 0, 65, 66, 
	65, 67, 63, 68, 63, 63, 63, 0, 
	69, 70, 69, 0, 65, 66, 65, 67, 
	69, 69, 69, 69, 0, 65, 66, 65, 
	67, 69, 69, 69, 69, 0, 65, 66, 
	65, 67, 63, 68, 63, 63, 63, 0, 
	71, 72, 73, 73, 74, 75, 76, 74, 
	0, 78, 79, 77, 80, 80, 81, 82, 
	0, 83, 83, 84, 0, 84, 84, 71, 
	73, 73, 76, 0, 76, 0, 80, 80, 
	81, 82, 85, 76, 0, 86, 0, 87, 
	0, 80, 80, 81, 82, 87, 0, 77, 
	88, 74, 74, 74, 74, 0, 88, 88, 
	89, 90, 90, 91, 0, 93, 94, 92, 
	95, 95, 81, 96, 97, 96, 0, 98, 
	98, 72, 74, 75, 74, 0, 88, 74, 
	74, 74, 74, 0, 92, 91, 0, 95, 
	95, 81, 99, 97, 91, 96, 0, 74, 
	88, 74, 100, 74, 74, 0, 95, 95, 
	81, 96, 101, 88, 74, 100, 74, 102, 
	0, 103, 104, 103, 0, 105, 105, 106, 
	103, 107, 103, 103, 103, 0, 108, 108, 
	109, 14, 0, 16, 0, 105, 105, 106, 
	103, 107, 103, 103, 103, 0, 6, 7, 
	6, 8, 9, 4, 10, 11, 4, 4, 
	4, 0, 111, 110, 1, 2, 1, 3, 
	4, 5, 4, 0, 112, 0
};

static const char _text_command_trans_targs[] = {
	0, 2, 74, 3, 4, 72, 5, 74, 
	45, 6, 38, 67, 5, 6, 38, 7, 
	32, 8, 9, 10, 16, 14, 21, 26, 
	15, 10, 11, 20, 12, 16, 13, 12, 
	13, 18, 17, 74, 3, 17, 19, 22, 
	23, 28, 29, 23, 24, 27, 25, 21, 
	26, 25, 30, 31, 26, 21, 33, 37, 
	17, 74, 3, 34, 35, 36, 39, 40, 
	44, 17, 74, 3, 41, 42, 43, 46, 
	52, 50, 56, 61, 51, 46, 47, 55, 
	48, 52, 49, 48, 49, 53, 74, 54, 
	57, 58, 63, 64, 58, 59, 62, 60, 
	56, 61, 60, 65, 66, 61, 56, 68, 
	71, 69, 70, 38, 69, 70, 73, 75, 
	0
};

static const char _text_command_trans_actions[] = {
	15, 0, 13, 0, 1, 1, 22, 58, 
	5, 22, 22, 22, 0, 0, 0, 0, 
	0, 1, 7, 3, 0, 19, 19, 19, 
	19, 19, 3, 19, 37, 9, 37, 3, 
	3, 19, 11, 40, 11, 0, 19, 3, 
	3, 19, 19, 19, 3, 19, 37, 62, 
	62, 3, 46, 19, 46, 46, 1, 1, 
	31, 50, 31, 28, 1, 1, 0, 1, 
	1, 34, 54, 34, 28, 1, 1, 3, 
	0, 19, 19, 19, 19, 19, 3, 19, 
	37, 9, 37, 3, 3, 19, 43, 19, 
	3, 3, 19, 19, 19, 3, 19, 37, 
	62, 62, 3, 46, 19, 46, 46, 1, 
	1, 25, 25, 25, 0, 0, 0, 17, 
	0
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
	15, 0, 0, 0
};

static const int text_command_start = 1;
static const int text_command_first_final = 74;

static const int text_command_en_eat_line = 73;
static const int text_command_en_main = 1;

#line 15 "src/core/text_command.rl"


void TextCommand::initialize() {
  int cs;
  
  silent_     = false;    
  clear();
  
#line 261 "src/core/text_command.cpp"
	{
	cs = text_command_start;
	}
#line 23 "src/core/text_command.rl"
  current_state_ = cs;
}

void TextCommand::do_listen() {
  char buffer[1024];
  char * line = buffer;
  
  if (!silent_) *output_ << "Welcome to rubyk !\n\n";
    
  clear();
  
  while(should_run() && getline(&line,1023)) {
    parse(line);
    parse("\n");
    if (should_run()) saveline(line); // command was not a 'quit'
    freeline(line);
  }
}

void TextCommand::parse(const std::string &string) {
  const char *p  = string.data();     // data pointer
  const char *pe = p + string.size(); // past end
  const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
  int cs = current_state_;            // restore machine state
  
  DEBUG(printf("parse:\"%s\"\n",string.c_str()));
  
  
#line 294 "src/core/text_command.cpp"
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
#line 51 "src/core/text_command.rl"
	{
      DEBUG(printf("_%c_",(*p)));
      token_ += (*p); /* append */
    }
	break;
	case 1:
#line 56 "src/core/text_command.rl"
	{
      // FIXME: this is a temporary hack until we sub parse with Value...
      parameter_string_ += (*p);
    }
	break;
	case 2:
#line 61 "src/core/text_command.rl"
	{ set_from_token(var_);}
	break;
	case 3:
#line 63 "src/core/text_command.rl"
	{ set_from_token(method_);}
	break;
	case 4:
#line 65 "src/core/text_command.rl"
	{ set_from_token(class_);}
	break;
	case 5:
#line 67 "src/core/text_command.rl"
	{ set_from_token(value_);}
	break;
	case 6:
#line 69 "src/core/text_command.rl"
	{ from_node_     = var_; }
	break;
	case 7:
#line 71 "src/core/text_command.rl"
	{ from_port_ = var_; }
	break;
	case 8:
#line 73 "src/core/text_command.rl"
	{ to_port_   = var_; }
	break;
	case 9:
#line 79 "src/core/text_command.rl"
	{
      to_node_   = var_;
      create_link();
    }
	break;
	case 10:
#line 84 "src/core/text_command.rl"
	{
      to_node_   = var_;
      remove_link();
    }
	break;
	case 11:
#line 89 "src/core/text_command.rl"
	{ create_instance(); }
	break;
	case 12:
#line 95 "src/core/text_command.rl"
	{ execute_command(); }
	break;
	case 13:
#line 99 "src/core/text_command.rl"
	{
      clear();
    }
	break;
	case 14:
#line 103 "src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 73; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 15:
#line 112 "src/core/text_command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 453 "src/core/text_command.cpp"
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
#line 103 "src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 73; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 480 "src/core/text_command.cpp"
		}
	}
	}

	_out: {}
	}
#line 164 "src/core/text_command.rl"

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
  
  ListValue list;
  names_to_urls();
  
  DEBUG(std::cout << "NEW "<< var_ << " = " << class_ << "(" << params << ")");
  list.push_back(var_);
  list.push_back(params);
  
  Value res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/new"), list);
  
  if (res.is_string() && !silent_) {
    print_result(root_->call(INSPECT_URL, res));
  } else if (!silent_) {
    print_result(res);
  }
}

void TextCommand::change_link(char op) {
  names_to_urls();
  
  DEBUG(std::cout << op << " LINK " << from_node_ << "." << from_port_ << "=>" << to_port_ << "." << to_node_ << std::endl);
  ListValue list;
  Value res;
  
  if (from_port_ == "") {
    list.push_back(std::string(from_node_).append("/out"));
  } else {
    list.push_back(std::string(from_node_).append("/out/").append(from_port_));
  }
  if (to_port_ == "") {
    list.push_back(std::string(to_node_).append("/in"));
  } else {
    list.push_back(std::string(to_node_).append("/in/").append(to_port_));
  }
  if (op == 'c') {
    res = root_->call(LINK_URL, list);
  } else {
    res = root_->call(UNLINK_URL, list);
  }
  print_result(res);
}

void TextCommand::execute_method() {
  Value res;
  // why doesn't this work ? Value params(Json(parameter_string_));
  Value params = Value(Json(parameter_string_));
  names_to_urls();
  
  DEBUG(std::cout << "METHOD " << var_ << "." << method_ << "(" << params << ")" << std::endl);
  
  if (method_ == "set") {
    // TODO: should 'set' live in normal tree space ?
    Object *target = root_->object_at(var_);
    if (target) {
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
  if (method_ == "set_lib_path") {
    res = root_->call(std::string(CLASS_URL).append("/lib_path"), params);
  } else if (method_ == "quit" || method_ == "q") {
    stop(); // Readline won't quit with a SIGTERM (see doc/prototypes/term_readline.cpp).
    res = root_->call(QUIT_URL);
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
