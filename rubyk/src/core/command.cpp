#line 1 "src/core/command.rl"
#include "command.h"
#include "rubyk.h"

//#define DEBUG_PARSER


#line 9 "src/core/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 13, 1, 14, 1, 15, 1, 
	17, 1, 18, 1, 19, 2, 1, 6, 
	2, 1, 7, 2, 1, 8, 2, 1, 
	11, 2, 1, 12, 2, 2, 14, 2, 
	2, 15, 2, 5, 9, 2, 5, 10, 
	2, 13, 17, 2, 14, 17, 2, 15, 
	17, 2, 16, 17, 3, 1, 11, 17, 
	3, 1, 12, 17, 3, 2, 14, 17, 
	3, 2, 15, 17, 3, 2, 16, 17, 
	3, 5, 10, 0, 4, 2, 14, 1, 
	7
};

static const short _command_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 22, 34, 
	37, 45, 47, 51, 54, 61, 63, 70, 
	74, 76, 82, 82, 90, 97, 99, 104, 
	109, 109, 111, 119, 121, 128, 142, 146, 
	147, 151, 163, 165, 176, 181, 185, 193, 
	201, 203, 207, 210, 217, 219, 226, 230, 
	232, 238, 238, 246, 253, 255, 260, 265, 
	265, 267, 275, 277, 284, 288, 300, 302, 
	313, 321, 323, 327, 330, 337, 339, 346, 
	347, 349, 355, 355, 363, 370, 372, 377, 
	382, 382, 384, 392, 394, 401, 403, 417, 
	422, 423, 431, 433, 437, 440, 447, 449, 
	456, 460, 462, 468, 468, 476, 483, 485, 
	490, 495, 495, 497, 505, 507, 514, 515, 
	523
};

static const char _command_trans_keys[] = {
	9, 10, 32, 35, 65, 90, 97, 122, 
	9, 10, 32, 10, 46, 95, 48, 57, 
	65, 90, 97, 122, 97, 122, 9, 10, 
	32, 35, 40, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 35, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 43, 45, 48, 57, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 9, 10, 
	32, 35, 48, 57, 9, 32, 41, 44, 
	48, 57, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 34, 43, 45, 48, 
	57, 34, 92, 9, 32, 41, 97, 122, 
	9, 32, 41, 97, 122, 48, 57, 9, 
	32, 41, 46, 48, 57, 97, 122, 48, 
	57, 9, 32, 41, 48, 57, 97, 122, 
	9, 10, 32, 40, 46, 47, 61, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	47, 61, 47, 9, 32, 97, 122, 9, 
	10, 32, 35, 46, 95, 48, 57, 65, 
	90, 97, 122, 97, 122, 9, 10, 32, 
	35, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 62, 65, 90, 9, 32, 65, 
	90, 40, 95, 48, 57, 65, 90, 97, 
	122, 34, 41, 43, 45, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 43, 45, 48, 
	57, 48, 57, 9, 32, 41, 44, 46, 
	48, 57, 9, 10, 32, 35, 48, 57, 
	9, 32, 41, 44, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 48, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 9, 32, 97, 122, 
	9, 10, 32, 35, 46, 95, 48, 57, 
	65, 90, 97, 122, 97, 122, 9, 10, 
	32, 35, 95, 48, 57, 65, 90, 97, 
	122, 34, 41, 43, 45, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 43, 45, 48, 
	57, 48, 57, 9, 32, 41, 44, 46, 
	48, 57, 10, 48, 57, 9, 32, 41, 
	44, 48, 57, 58, 95, 48, 57, 65, 
	90, 97, 122, 9, 32, 34, 43, 45, 
	48, 57, 34, 92, 9, 32, 41, 97, 
	122, 9, 32, 41, 97, 122, 48, 57, 
	9, 32, 41, 46, 48, 57, 97, 122, 
	48, 57, 9, 32, 41, 48, 57, 97, 
	122, 97, 122, 9, 10, 32, 35, 40, 
	47, 61, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 35, 47, 61, 62, 34, 
	41, 43, 45, 48, 57, 97, 122, 34, 
	92, 9, 32, 41, 44, 9, 32, 44, 
	9, 32, 34, 43, 45, 48, 57, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	9, 10, 32, 35, 48, 57, 9, 32, 
	41, 44, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 34, 43, 
	45, 48, 57, 34, 92, 9, 32, 41, 
	97, 122, 9, 32, 41, 97, 122, 48, 
	57, 9, 32, 41, 46, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 48, 57, 
	97, 122, 10, 9, 10, 32, 35, 65, 
	90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 4, 3, 1, 2, 0, 6, 3, 
	4, 2, 4, 3, 5, 0, 5, 4, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 8, 4, 1, 
	2, 6, 0, 5, 3, 2, 2, 4, 
	2, 4, 3, 5, 0, 5, 4, 0, 
	4, 0, 2, 5, 2, 3, 3, 0, 
	0, 4, 0, 3, 2, 6, 0, 5, 
	4, 2, 4, 3, 5, 0, 5, 1, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 0, 8, 5, 
	1, 4, 2, 4, 3, 5, 0, 5, 
	4, 0, 4, 0, 2, 5, 2, 3, 
	3, 0, 0, 4, 0, 3, 1, 4, 
	0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 0, 3, 1, 3, 0, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 3, 0, 0, 
	1, 3, 1, 3, 1, 1, 3, 2, 
	0, 0, 0, 1, 1, 1, 0, 1, 
	1, 0, 3, 1, 0, 1, 1, 0, 
	1, 2, 1, 2, 1, 3, 1, 3, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 1, 3, 0, 
	0, 2, 0, 0, 0, 1, 1, 1, 
	0, 1, 1, 0, 3, 1, 0, 1, 
	1, 0, 1, 2, 1, 2, 0, 2, 
	0
};

static const short _command_index_offsets[] = {
	0, 0, 7, 11, 13, 19, 21, 31, 
	35, 42, 45, 50, 54, 61, 63, 70, 
	75, 77, 83, 84, 90, 97, 100, 105, 
	110, 111, 113, 120, 122, 128, 140, 145, 
	147, 151, 161, 163, 172, 177, 181, 187, 
	194, 197, 202, 206, 213, 215, 222, 227, 
	229, 235, 236, 242, 249, 252, 257, 262, 
	263, 265, 272, 274, 280, 284, 294, 296, 
	305, 312, 315, 320, 324, 331, 333, 340, 
	342, 344, 350, 351, 357, 364, 367, 372, 
	377, 378, 380, 387, 389, 395, 397, 409, 
	415, 417, 424, 427, 432, 436, 443, 445, 
	452, 457, 459, 465, 466, 472, 479, 482, 
	487, 492, 493, 495, 502, 504, 510, 512, 
	519
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 5, 0, 1, 
	2, 1, 0, 2, 3, 6, 4, 4, 
	4, 4, 0, 7, 0, 8, 9, 8, 
	10, 11, 7, 7, 7, 7, 0, 12, 
	12, 3, 0, 13, 14, 15, 15, 16, 
	17, 0, 19, 20, 18, 21, 21, 22, 
	23, 0, 24, 24, 25, 0, 25, 25, 
	13, 15, 15, 16, 0, 16, 0, 21, 
	21, 22, 23, 26, 16, 0, 27, 28, 
	27, 29, 0, 30, 0, 21, 21, 22, 
	23, 30, 0, 18, 31, 17, 17, 17, 
	17, 0, 32, 32, 33, 34, 34, 35, 
	0, 37, 38, 36, 39, 39, 40, 41, 
	0, 42, 42, 14, 17, 0, 36, 35, 
	0, 39, 39, 40, 43, 35, 41, 0, 
	44, 0, 39, 39, 40, 44, 41, 0, 
	45, 46, 45, 47, 48, 49, 50, 5, 
	5, 5, 5, 0, 51, 51, 52, 53, 
	0, 54, 0, 54, 54, 55, 0, 56, 
	57, 56, 58, 59, 55, 55, 55, 55, 
	0, 60, 0, 56, 57, 56, 58, 60, 
	60, 60, 60, 0, 61, 61, 62, 63, 
	0, 61, 61, 63, 0, 64, 63, 63, 
	63, 63, 0, 65, 66, 67, 67, 68, 
	69, 0, 71, 72, 70, 73, 73, 74, 
	75, 0, 76, 76, 77, 0, 77, 77, 
	65, 67, 67, 68, 0, 68, 0, 73, 
	73, 74, 75, 78, 68, 0, 79, 80, 
	79, 81, 0, 82, 0, 73, 73, 74, 
	75, 82, 0, 70, 83, 69, 69, 69, 
	69, 0, 84, 84, 85, 86, 86, 87, 
	0, 89, 90, 88, 91, 91, 92, 93, 
	0, 94, 94, 66, 69, 0, 88, 87, 
	0, 91, 91, 92, 95, 87, 93, 0, 
	96, 0, 91, 91, 92, 96, 93, 0, 
	62, 62, 97, 0, 98, 99, 98, 100, 
	101, 97, 97, 97, 97, 0, 102, 0, 
	98, 99, 98, 100, 102, 102, 102, 102, 
	0, 103, 104, 105, 105, 106, 107, 0, 
	109, 110, 108, 111, 111, 112, 113, 0, 
	114, 114, 115, 0, 115, 115, 103, 105, 
	105, 106, 0, 106, 0, 111, 111, 112, 
	113, 116, 106, 0, 117, 0, 118, 0, 
	111, 111, 112, 113, 118, 0, 108, 119, 
	107, 107, 107, 107, 0, 120, 120, 121, 
	122, 122, 123, 0, 125, 126, 124, 127, 
	127, 128, 129, 0, 130, 130, 104, 107, 
	0, 124, 123, 0, 127, 127, 128, 131, 
	123, 129, 0, 132, 0, 127, 127, 128, 
	132, 129, 0, 133, 0, 134, 135, 134, 
	136, 137, 138, 139, 133, 133, 133, 133, 
	0, 140, 140, 3, 52, 141, 0, 62, 
	0, 142, 143, 144, 144, 145, 146, 0, 
	148, 149, 147, 150, 150, 151, 152, 0, 
	153, 153, 154, 0, 154, 154, 142, 144, 
	144, 145, 0, 145, 0, 150, 150, 151, 
	152, 155, 145, 0, 156, 157, 156, 158, 
	0, 159, 0, 150, 150, 151, 152, 159, 
	0, 147, 160, 146, 146, 146, 146, 0, 
	161, 161, 162, 163, 163, 164, 0, 166, 
	167, 165, 168, 168, 169, 170, 0, 171, 
	171, 143, 146, 0, 165, 164, 0, 168, 
	168, 169, 172, 164, 170, 0, 173, 0, 
	168, 168, 169, 173, 170, 0, 175, 174, 
	1, 2, 1, 3, 4, 5, 0, 176, 
	0
};

static const char _command_trans_targs[] = {
	0, 2, 111, 3, 4, 29, 5, 6, 
	7, 111, 3, 8, 7, 9, 15, 13, 
	14, 19, 9, 10, 18, 11, 15, 12, 
	11, 12, 16, 7, 111, 3, 17, 20, 
	20, 21, 25, 26, 21, 22, 24, 23, 
	15, 19, 23, 27, 28, 30, 111, 64, 
	85, 31, 36, 30, 31, 36, 32, 33, 
	7, 111, 3, 34, 35, 37, 60, 38, 
	39, 40, 46, 44, 45, 50, 40, 41, 
	49, 42, 46, 43, 42, 43, 47, 7, 
	111, 3, 48, 51, 51, 52, 56, 57, 
	52, 53, 55, 54, 46, 50, 54, 58, 
	59, 61, 7, 111, 3, 62, 63, 65, 
	71, 69, 70, 75, 65, 66, 74, 67, 
	71, 68, 67, 68, 72, 111, 73, 76, 
	76, 77, 81, 82, 77, 78, 80, 79, 
	71, 75, 79, 83, 84, 86, 87, 111, 
	3, 89, 31, 88, 87, 88, 90, 96, 
	94, 95, 100, 90, 91, 99, 92, 96, 
	93, 92, 93, 97, 7, 111, 3, 98, 
	101, 101, 102, 106, 107, 102, 103, 105, 
	104, 96, 100, 104, 108, 109, 110, 112, 
	0
};

static const char _command_trans_actions[] = {
	17, 0, 15, 0, 1, 1, 7, 1, 
	39, 72, 39, 3, 0, 0, 0, 1, 
	1, 1, 1, 0, 1, 42, 42, 42, 
	0, 0, 1, 13, 54, 13, 1, 5, 
	0, 0, 1, 1, 1, 0, 1, 45, 
	45, 80, 0, 1, 1, 21, 76, 3, 
	21, 21, 21, 0, 0, 0, 0, 1, 
	33, 64, 33, 27, 1, 0, 0, 1, 
	7, 0, 0, 1, 1, 1, 1, 0, 
	1, 42, 42, 42, 0, 0, 1, 9, 
	48, 9, 1, 5, 0, 0, 1, 1, 
	1, 0, 1, 45, 45, 80, 0, 1, 
	1, 1, 30, 60, 30, 27, 1, 0, 
	0, 1, 1, 1, 1, 0, 1, 42, 
	42, 42, 0, 0, 1, 57, 1, 5, 
	0, 0, 1, 1, 1, 0, 1, 45, 
	45, 80, 0, 1, 1, 1, 84, 68, 
	36, 3, 24, 24, 0, 0, 0, 0, 
	1, 1, 1, 1, 0, 1, 42, 42, 
	42, 0, 0, 1, 11, 51, 11, 1, 
	5, 0, 0, 1, 1, 1, 0, 1, 
	45, 45, 80, 0, 1, 1, 0, 19, 
	0
};

static const char _command_eof_actions[] = {
	0, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 0, 0, 
	0
};

static const int command_start = 1;
static const int command_first_final = 111;

static const int command_en_eat_line = 110;
static const int command_en_main = 1;

#line 9 "src/core/command.rl"


/*
  typedef enum command_actions_ {
    CmdNoAction,
  	CmdCreateNode,
  	CmdCreateLink,
  	CmdOpenGroup,
  } command_action_t;
*/

void Command::initialize()
{
  int cs;
  
  mAction = CmdNoAction;
  mTree       = NULL;
  mQuit       = false;
  mTokenIndex = 0;
  mSilent     = false;
  mThread     = NULL;
  
  clear();
  
  
#line 337 "src/core/command.cpp"
	{
	cs = command_start;
	}
#line 34 "src/core/command.rl"
  mCurrentState = cs;
}

int Command::do_listen()
{
  char buffer[1024];
  char * line = buffer;
  
  // set thread priority to normal
  mTree->normal_priority();
  
  if (!mSilent)
    *mOutput << "Welcome to rubyk !\n\n";
  clear();

  while(!mQuit && getline(&line,1023)) {
    parse(line);
    parse("\n");
    if (!mQuit)
      saveline(line);
    freeline(line);
  }
  return 0; // thread return value
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  const char *eof = NULL;  // FIXME: this should be set to 'pe' on the last pStr block...
  int cs = mCurrentState;        // restore machine state
  
  
#line 375 "src/core/command.cpp"
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
	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
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

	_klen = _command_range_lengths[cs];
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
	_trans = _command_indicies[_trans];
	cs = _command_trans_targs[_trans];

	if ( _command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 67 "src/core/command.rl"
	{
      if (mTokenIndex >= MAX_TOKEN_SIZE) {
        std::cerr << "Buffer overflow !" << std::endl;
        // stop parsing
        return;
      }
#ifdef DEBUG_PARSER
      printf("_%c_",(*p));
#endif
      mToken[mTokenIndex] = (*p); /* append */
      mTokenIndex++;     
    }
	break;
	case 1:
#line 80 "src/core/command.rl"
	{ set_from_token(mVar);}
	break;
	case 2:
#line 82 "src/core/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 84 "src/core/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 86 "src/core/command.rl"
	{ set_from_token(mClass);}
	break;
	case 5:
#line 88 "src/core/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 90 "src/core/command.rl"
	{ mFrom     = mVar; }
	break;
	case 7:
#line 92 "src/core/command.rl"
	{ mFromPort = mVar; }
	break;
	case 8:
#line 94 "src/core/command.rl"
	{ mToPort   = mVar; }
	break;
	case 9:
#line 96 "src/core/command.rl"
	{ 
      add_value_from_token(); 
    }
	break;
	case 10:
#line 100 "src/core/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 102 "src/core/command.rl"
	{
      mTo   = mVar;
      create_link();
    }
	break;
	case 12:
#line 107 "src/core/command.rl"
	{
      mTo   = mVar;
      remove_link();
    }
	break;
	case 13:
#line 112 "src/core/command.rl"
	{ create_instance(); }
	break;
	case 14:
#line 114 "src/core/command.rl"
	{ execute_method(); }
	break;
	case 15:
#line 116 "src/core/command.rl"
	{ execute_class_method(); }
	break;
	case 16:
#line 118 "src/core/command.rl"
	{ execute_command(); }
	break;
	case 17:
#line 122 "src/core/command.rl"
	{
      if (!mQuit) {
        clear();
      }
    }
	break;
	case 18:
#line 127 "src/core/command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 110; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 19:
#line 136 "src/core/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 559 "src/core/command.cpp"
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
	const char *__acts = _command_actions + _command_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 18:
#line 127 "src/core/command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 110; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 586 "src/core/command.cpp"
		}
	}
	}

	_out: {}
	}
#line 188 "src/core/command.rl"

//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close()
{
  mQuit = true;
  if (mThread) {
    pthread_kill(mThread, SIGINT);
  }
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
#ifdef DEBUG_PARSER
  if (&pElem == &mValue) std::cout << "[val " << mToken << "]" << std::endl;
  if (&pElem == &mVar)   std::cout << "[var " << mToken << "]" << std::endl;
  if (&pElem == &mClass) std::cout << "[cla " << mToken << "]" << std::endl;
#endif
  pElem = mToken;
  mTokenIndex = 0;
}

void Command::add_value_from_token () 
{
  // FIXME: what do we do here ? 
  // mParameters.add(mValue); // add to 'list' value
}

void Command::set_parameter  (const std::string& pKey, const std::string& pValue) 
{
  mParameters.set_key(pKey,Value(pValue));
}

// FIXME: create_instance should run in server space with concurrency locks.
void Command::create_instance()
{
  mTree->lock();
    // FIXME: Group scope
    // FIXME: should be new_object(mVar, mClass, Value(mParams))
    Value res = mTree->new_object(mVar, mClass, mParameters);
  mTree->unlock();

#ifdef DEBUG_PARSER
  std::cout << "NEW "<< mVar << " = " << mClass << "(" << mParameters << ")";
#endif

  if (!mSilent)
    *mOutput << res << std::endl;
}


void Command::create_link()
{ 
  mTree->lock();
    // FIXME: Group scope
    mTree->create_link(mFrom, mFromPort, mToPort, mTo);
  mTree->unlock();

#ifdef DEBUG_PARSER
  std::cout << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

void Command::remove_link()
{ 
  mTree->lock();
    // FIXME: Group scope
    mTree->remove_link(mFrom, mFromPort, mToPort, mTo);
  mTree->unlock();

#ifdef DEBUG_PARSER
  std::cout << "UNLINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Value res;
  mTree->lock();
    // FIXME: Group scope
    res = mTree->call(std::string("/").append(mVar).append("/").append(mMethod), mParameters);
  mTree->unlock();
  
  *mOutput << res << std::endl;
}

void Command::execute_class_method()
{
  Value res;
  mTree->lock();
    res = mTree->call(std::string(CLASS_ROOT).append("/").append(mClass).append("/").append(mMethod), mParameters);
  mTree->unlock();
  
  *mOutput << res << std::endl;
}

void Command::execute_command()
{
  Value res;
  mTree->lock();
    // FIXME: Group scope
    res = mTree->call(std::string("/").append(mMethod));
  mTree->unlock();
  *mOutput << res << std::endl;
  /*
  TODO: these methods should exist in root...
  
  if (mTree->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
    
  } else if (mMethod == "quit" || mMethod == "q") {
    mTree->quit();
    mQuit = true;
  } else if (mMethod == "set_lib_path") {
    std::string path;
    if (!mParameters.get(&path)) {
      *mOutput << "Could not set library path (no parameter).\n";
    } else {
      Class::set_lib_path(path);
      *mOutput << "Library path set to '" << path << "'.\n";
    }
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  */
}

void Command::clear() 
{
  mTokenIndex = 0;
  mAction     = CmdNoAction;
  mVar        = "";
  mClass      = "";
  mParameters.clear();
  mFromPort = 1;
  mToPort   = 1;
}
