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
	4, 1, 5, 1, 11, 1, 12, 1, 
	13, 1, 15, 1, 16, 1, 17, 2, 
	0, 1, 2, 2, 6, 2, 2, 7, 
	2, 2, 8, 2, 2, 9, 2, 2, 
	10, 2, 3, 12, 2, 3, 13, 2, 
	5, 1, 2, 11, 15, 2, 12, 15, 
	2, 13, 15, 2, 14, 15, 3, 2, 
	9, 15, 3, 2, 10, 15, 3, 3, 
	12, 15, 3, 3, 13, 15, 3, 3, 
	14, 15, 3, 5, 0, 1, 4, 3, 
	12, 2, 7
};

static const short _text_command_key_offsets[] = {
	0, 0, 9, 12, 13, 27, 31, 36, 
	40, 48, 57, 59, 63, 66, 73, 75, 
	82, 86, 89, 91, 97, 97, 105, 112, 
	114, 120, 126, 126, 128, 137, 139, 147, 
	152, 164, 167, 178, 179, 184, 196, 199, 
	210, 219, 221, 225, 228, 235, 237, 244, 
	245, 247, 253, 253, 261, 268, 270, 276, 
	282, 282, 284, 293, 295, 303, 306, 320, 
	325, 326, 335, 337, 341, 344, 351, 353, 
	360, 364, 366, 372, 372, 380, 387, 389, 
	395, 401, 401, 403, 412, 414, 422, 430, 
	433, 445, 454, 456, 460, 463, 470, 472, 
	479, 483, 485, 491, 491, 499, 506, 508, 
	514, 520, 520, 522, 531, 533, 541, 542, 
	551
};

static const char _text_command_trans_keys[] = {
	9, 10, 32, 35, 47, 65, 90, 97, 
	122, 9, 10, 32, 10, 9, 10, 32, 
	40, 46, 61, 95, 124, 47, 57, 65, 
	90, 97, 122, 9, 32, 61, 124, 9, 
	32, 62, 65, 90, 9, 32, 65, 90, 
	40, 95, 48, 57, 65, 90, 97, 122, 
	34, 41, 43, 45, 47, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 43, 45, 48, 
	57, 48, 57, 9, 32, 41, 44, 46, 
	48, 57, 9, 10, 32, 35, 9, 32, 
	35, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 47, 57, 65, 90, 97, 
	122, 9, 32, 34, 43, 45, 48, 57, 
	34, 92, 9, 32, 41, 47, 97, 122, 
	9, 32, 41, 47, 97, 122, 48, 57, 
	9, 32, 41, 46, 47, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 47, 48, 
	57, 97, 122, 9, 32, 47, 97, 122, 
	9, 10, 32, 35, 46, 95, 47, 57, 
	65, 90, 97, 122, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 124, 9, 32, 47, 97, 122, 
	9, 10, 32, 35, 46, 95, 47, 57, 
	65, 90, 97, 122, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 34, 41, 43, 45, 47, 48, 
	57, 97, 122, 34, 92, 9, 32, 41, 
	44, 9, 32, 44, 9, 32, 34, 43, 
	45, 48, 57, 48, 57, 9, 32, 41, 
	44, 46, 48, 57, 10, 48, 57, 9, 
	32, 41, 44, 48, 57, 58, 95, 47, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 57, 34, 92, 9, 32, 
	41, 47, 97, 122, 9, 32, 41, 47, 
	97, 122, 48, 57, 9, 32, 41, 46, 
	47, 48, 57, 97, 122, 48, 57, 9, 
	32, 41, 47, 48, 57, 97, 122, 47, 
	97, 122, 9, 10, 32, 35, 40, 61, 
	95, 124, 47, 57, 65, 90, 97, 122, 
	9, 32, 35, 61, 124, 62, 34, 41, 
	43, 45, 47, 48, 57, 97, 122, 34, 
	92, 9, 32, 41, 44, 9, 32, 44, 
	9, 32, 34, 43, 45, 48, 57, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	9, 10, 32, 35, 48, 57, 9, 32, 
	41, 44, 48, 57, 58, 95, 47, 57, 
	65, 90, 97, 122, 9, 32, 34, 43, 
	45, 48, 57, 34, 92, 9, 32, 41, 
	47, 97, 122, 9, 32, 41, 47, 97, 
	122, 48, 57, 9, 32, 41, 46, 47, 
	48, 57, 97, 122, 48, 57, 9, 32, 
	41, 47, 48, 57, 97, 122, 46, 95, 
	48, 57, 65, 90, 97, 122, 47, 97, 
	122, 9, 10, 32, 35, 40, 95, 47, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 47, 48, 57, 97, 122, 34, 92, 
	9, 32, 41, 44, 9, 32, 44, 9, 
	32, 34, 43, 45, 48, 57, 48, 57, 
	9, 32, 41, 44, 46, 48, 57, 9, 
	10, 32, 35, 48, 57, 9, 32, 41, 
	44, 48, 57, 58, 95, 47, 57, 65, 
	90, 97, 122, 9, 32, 34, 43, 45, 
	48, 57, 34, 92, 9, 32, 41, 47, 
	97, 122, 9, 32, 41, 47, 97, 122, 
	48, 57, 9, 32, 41, 46, 47, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	47, 48, 57, 97, 122, 10, 9, 10, 
	32, 35, 47, 65, 90, 97, 122, 0
};

static const char _text_command_single_lengths[] = {
	0, 5, 3, 1, 8, 4, 3, 2, 
	2, 5, 2, 4, 3, 5, 0, 5, 
	4, 3, 0, 4, 0, 2, 5, 2, 
	4, 4, 0, 0, 5, 0, 4, 3, 
	6, 1, 5, 1, 3, 6, 1, 5, 
	5, 2, 4, 3, 5, 0, 5, 1, 
	0, 4, 0, 2, 5, 2, 4, 4, 
	0, 0, 5, 0, 4, 1, 8, 5, 
	1, 5, 2, 4, 3, 5, 0, 5, 
	4, 0, 4, 0, 2, 5, 2, 4, 
	4, 0, 0, 5, 0, 4, 2, 1, 
	6, 5, 2, 4, 3, 5, 0, 5, 
	4, 0, 4, 0, 2, 5, 2, 4, 
	4, 0, 0, 5, 0, 4, 1, 5, 
	0
};

static const char _text_command_range_lengths[] = {
	0, 2, 0, 0, 3, 0, 1, 1, 
	3, 2, 0, 0, 0, 1, 1, 1, 
	0, 0, 1, 1, 0, 3, 1, 0, 
	1, 1, 0, 1, 2, 1, 2, 1, 
	3, 1, 3, 0, 1, 3, 1, 3, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 1, 3, 0, 
	0, 2, 0, 0, 0, 1, 1, 1, 
	0, 1, 1, 0, 3, 1, 0, 1, 
	1, 0, 1, 2, 1, 2, 3, 1, 
	3, 2, 0, 0, 0, 1, 1, 1, 
	0, 1, 1, 0, 3, 1, 0, 1, 
	1, 0, 1, 2, 1, 2, 0, 2, 
	0
};

static const short _text_command_index_offsets[] = {
	0, 0, 8, 12, 14, 26, 31, 36, 
	40, 46, 54, 57, 62, 66, 73, 75, 
	82, 87, 91, 93, 99, 100, 106, 113, 
	116, 122, 128, 129, 131, 139, 141, 148, 
	153, 163, 166, 175, 177, 182, 192, 195, 
	204, 212, 215, 220, 224, 231, 233, 240, 
	242, 244, 250, 251, 257, 264, 267, 273, 
	279, 280, 282, 290, 292, 299, 302, 314, 
	320, 322, 330, 333, 338, 342, 349, 351, 
	358, 363, 365, 371, 372, 378, 385, 388, 
	394, 400, 401, 403, 411, 413, 420, 426, 
	429, 439, 447, 450, 455, 459, 466, 468, 
	475, 480, 482, 488, 489, 495, 502, 505, 
	511, 517, 518, 520, 528, 530, 537, 539, 
	547
};

static const unsigned char _text_command_indicies[] = {
	1, 2, 1, 3, 4, 5, 4, 0, 
	1, 2, 1, 0, 2, 3, 6, 7, 
	6, 8, 9, 10, 4, 11, 4, 4, 
	4, 0, 12, 12, 13, 14, 0, 15, 
	15, 16, 17, 0, 15, 15, 17, 0, 
	18, 17, 17, 17, 17, 0, 19, 20, 
	21, 21, 22, 23, 22, 0, 25, 26, 
	24, 27, 27, 28, 29, 0, 30, 30, 
	31, 0, 31, 31, 19, 21, 21, 23, 
	0, 23, 0, 27, 27, 28, 29, 32, 
	23, 0, 33, 34, 33, 35, 0, 36, 
	36, 3, 0, 37, 0, 27, 27, 28, 
	29, 37, 0, 24, 38, 22, 22, 22, 
	22, 0, 38, 38, 39, 40, 40, 41, 
	0, 43, 44, 42, 45, 45, 28, 46, 
	46, 0, 47, 47, 20, 22, 22, 0, 
	42, 41, 0, 45, 45, 28, 48, 46, 
	41, 46, 0, 49, 0, 45, 45, 28, 
	46, 49, 46, 0, 16, 16, 50, 50, 
	0, 51, 52, 51, 53, 54, 50, 50, 
	50, 50, 0, 55, 55, 0, 51, 52, 
	51, 53, 55, 55, 55, 55, 0, 56, 
	0, 56, 56, 57, 57, 0, 58, 59, 
	58, 60, 61, 57, 57, 57, 57, 0, 
	62, 62, 0, 58, 59, 58, 60, 62, 
	62, 62, 62, 0, 63, 64, 65, 65, 
	66, 67, 66, 0, 69, 70, 68, 71, 
	71, 72, 73, 0, 74, 74, 75, 0, 
	75, 75, 63, 65, 65, 67, 0, 67, 
	0, 71, 71, 72, 73, 76, 67, 0, 
	77, 0, 78, 0, 71, 71, 72, 73, 
	78, 0, 68, 79, 66, 66, 66, 66, 
	0, 79, 79, 80, 81, 81, 82, 0, 
	84, 85, 83, 86, 86, 72, 87, 87, 
	0, 88, 88, 64, 66, 66, 0, 83, 
	82, 0, 86, 86, 72, 89, 87, 82, 
	87, 0, 90, 0, 86, 86, 72, 87, 
	90, 87, 0, 91, 91, 0, 92, 93, 
	92, 94, 95, 96, 91, 97, 91, 91, 
	91, 0, 98, 98, 3, 99, 14, 0, 
	16, 0, 100, 101, 102, 102, 103, 104, 
	103, 0, 106, 107, 105, 108, 108, 109, 
	110, 0, 111, 111, 112, 0, 112, 112, 
	100, 102, 102, 104, 0, 104, 0, 108, 
	108, 109, 110, 113, 104, 0, 114, 115, 
	114, 116, 0, 117, 0, 108, 108, 109, 
	110, 117, 0, 105, 118, 103, 103, 103, 
	103, 0, 118, 118, 119, 120, 120, 121, 
	0, 123, 124, 122, 125, 125, 109, 126, 
	126, 0, 127, 127, 101, 103, 103, 0, 
	122, 121, 0, 125, 125, 109, 128, 126, 
	121, 126, 0, 129, 0, 125, 125, 109, 
	126, 129, 126, 0, 130, 5, 5, 5, 
	5, 0, 131, 131, 0, 132, 133, 132, 
	134, 135, 131, 131, 131, 131, 0, 136, 
	137, 138, 138, 139, 140, 139, 0, 142, 
	143, 141, 144, 144, 145, 146, 0, 147, 
	147, 148, 0, 148, 148, 136, 138, 138, 
	140, 0, 140, 0, 144, 144, 145, 146, 
	149, 140, 0, 150, 151, 150, 152, 0, 
	153, 0, 144, 144, 145, 146, 153, 0, 
	141, 154, 139, 139, 139, 139, 0, 154, 
	154, 155, 156, 156, 157, 0, 159, 160, 
	158, 161, 161, 145, 162, 162, 0, 163, 
	163, 137, 139, 139, 0, 158, 157, 0, 
	161, 161, 145, 164, 162, 157, 162, 0, 
	165, 0, 161, 161, 145, 162, 165, 162, 
	0, 167, 166, 1, 2, 1, 3, 4, 
	5, 4, 0, 168, 0
};

static const char _text_command_trans_targs[] = {
	0, 2, 111, 3, 4, 86, 5, 111, 
	40, 61, 6, 35, 5, 6, 35, 7, 
	31, 8, 9, 10, 16, 14, 21, 15, 
	10, 11, 20, 12, 16, 13, 12, 13, 
	18, 17, 111, 3, 17, 19, 22, 23, 
	27, 28, 23, 24, 26, 25, 21, 25, 
	29, 30, 32, 17, 111, 3, 33, 34, 
	36, 37, 17, 111, 3, 38, 39, 41, 
	47, 45, 51, 46, 41, 42, 50, 43, 
	47, 44, 43, 44, 48, 111, 49, 52, 
	53, 57, 58, 53, 54, 56, 55, 51, 
	55, 59, 60, 62, 63, 111, 3, 65, 
	64, 35, 63, 64, 66, 72, 70, 76, 
	71, 66, 67, 75, 68, 72, 69, 68, 
	69, 73, 17, 111, 3, 74, 77, 78, 
	82, 83, 78, 79, 81, 80, 76, 80, 
	84, 85, 87, 88, 17, 111, 3, 89, 
	90, 96, 94, 100, 95, 90, 91, 99, 
	92, 96, 93, 92, 93, 97, 17, 111, 
	3, 98, 101, 102, 106, 107, 102, 103, 
	105, 104, 100, 104, 108, 109, 110, 112, 
	0
};

static const char _text_command_trans_actions[] = {
	19, 0, 17, 0, 1, 1, 26, 78, 
	5, 26, 26, 26, 0, 0, 0, 0, 
	0, 1, 7, 3, 0, 23, 23, 23, 
	23, 3, 23, 47, 9, 47, 3, 3, 
	23, 11, 50, 11, 0, 23, 3, 3, 
	23, 23, 23, 3, 23, 47, 82, 3, 
	23, 23, 1, 35, 62, 35, 32, 1, 
	0, 1, 38, 66, 38, 32, 1, 3, 
	0, 23, 23, 23, 23, 3, 23, 47, 
	9, 47, 3, 3, 23, 59, 23, 3, 
	3, 23, 23, 23, 3, 23, 47, 82, 
	3, 23, 23, 1, 86, 70, 41, 5, 
	29, 29, 0, 0, 3, 0, 23, 23, 
	23, 23, 3, 23, 47, 9, 47, 3, 
	3, 23, 13, 53, 13, 23, 3, 3, 
	23, 23, 23, 3, 23, 47, 82, 3, 
	23, 23, 7, 1, 44, 74, 44, 5, 
	3, 0, 23, 23, 23, 23, 3, 23, 
	47, 9, 47, 3, 3, 23, 15, 56, 
	15, 23, 3, 3, 23, 23, 23, 3, 
	23, 47, 82, 3, 23, 23, 0, 21, 
	0
};

static const char _text_command_eof_actions[] = {
	0, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 0, 0, 
	0
};

static const int text_command_start = 1;
static const int text_command_first_final = 111;

static const int text_command_en_eat_line = 110;
static const int text_command_en_main = 1;

#line 15 "src/core/text_command.rl"


void TextCommand::initialize() {
  int cs;
  
  silent_     = false;    
  clear();
  
#line 330 "src/core/text_command.cpp"
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
  
  
#line 363 "src/core/text_command.cpp"
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
#line 91 "src/core/text_command.rl"
	{ execute_method(); }
	break;
	case 13:
#line 93 "src/core/text_command.rl"
	{ execute_class_method(); }
	break;
	case 14:
#line 95 "src/core/text_command.rl"
	{ execute_command(); }
	break;
	case 15:
#line 99 "src/core/text_command.rl"
	{
      clear();
    }
	break;
	case 16:
#line 103 "src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 110; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 17:
#line 112 "src/core/text_command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 530 "src/core/text_command.cpp"
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
	case 16:
#line 103 "src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 110; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 557 "src/core/text_command.cpp"
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
    method_.insert(0, current_directory_);
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
