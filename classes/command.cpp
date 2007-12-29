#line 1 "classes/command.rl"
#include "command.h"
#include "rubyk.h"
#include "class.h"
#include "mutex.h"

//#define DEBUG_PARSER


#line 11 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 7, 1, 8, 1, 13, 1, 
	14, 1, 15, 1, 17, 1, 18, 1, 
	19, 2, 1, 6, 2, 1, 11, 2, 
	1, 12, 2, 2, 14, 2, 2, 15, 
	2, 5, 9, 2, 5, 10, 2, 13, 
	17, 2, 14, 17, 2, 15, 17, 2, 
	16, 17, 3, 1, 11, 17, 3, 1, 
	12, 17, 3, 2, 14, 17, 3, 2, 
	15, 17, 3, 2, 16, 17, 3, 5, 
	10, 0
};

static const short _command_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 22, 33, 
	34, 42, 44, 48, 51, 58, 60, 67, 
	70, 72, 78, 78, 86, 93, 95, 100, 
	105, 105, 107, 115, 117, 124, 138, 142, 
	143, 151, 153, 156, 158, 168, 173, 177, 
	185, 193, 195, 199, 202, 209, 211, 218, 
	221, 223, 229, 229, 237, 244, 246, 251, 
	256, 256, 258, 266, 268, 275, 283, 285, 
	288, 290, 300, 308, 310, 314, 317, 324, 
	326, 333, 334, 336, 342, 342, 350, 357, 
	359, 364, 369, 369, 371, 379, 381, 388, 
	394, 396, 402, 406, 407, 418, 426, 428, 
	432, 435, 442, 444, 451, 454, 456, 462, 
	462, 470, 477, 479, 484, 489, 489, 491, 
	499, 501, 508, 509, 517
};

static const char _command_trans_keys[] = {
	9, 10, 32, 35, 65, 90, 97, 122, 
	9, 10, 32, 10, 46, 95, 48, 57, 
	65, 90, 97, 122, 97, 122, 9, 10, 
	32, 40, 95, 48, 57, 65, 90, 97, 
	122, 35, 34, 41, 43, 45, 48, 57, 
	97, 122, 34, 92, 9, 32, 41, 44, 
	9, 32, 44, 9, 32, 34, 43, 45, 
	48, 57, 48, 57, 9, 32, 41, 44, 
	46, 48, 57, 9, 10, 32, 48, 57, 
	9, 32, 41, 44, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 48, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 9, 10, 32, 40, 
	46, 47, 61, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 47, 61, 47, 9, 
	32, 43, 45, 48, 57, 97, 122, 48, 
	57, 46, 48, 57, 97, 122, 9, 10, 
	32, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 62, 65, 90, 9, 32, 65, 
	90, 40, 95, 48, 57, 65, 90, 97, 
	122, 34, 41, 43, 45, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 43, 45, 48, 
	57, 48, 57, 9, 32, 41, 44, 46, 
	48, 57, 9, 10, 32, 48, 57, 9, 
	32, 41, 44, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 97, 122, 
	48, 57, 9, 32, 41, 46, 48, 57, 
	97, 122, 48, 57, 9, 32, 41, 48, 
	57, 97, 122, 9, 32, 43, 45, 48, 
	57, 97, 122, 48, 57, 46, 48, 57, 
	97, 122, 9, 10, 32, 95, 48, 57, 
	65, 90, 97, 122, 34, 41, 43, 45, 
	48, 57, 97, 122, 34, 92, 9, 32, 
	41, 44, 9, 32, 44, 9, 32, 34, 
	43, 45, 48, 57, 48, 57, 9, 32, 
	41, 44, 46, 48, 57, 10, 48, 57, 
	9, 32, 41, 44, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 48, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 43, 45, 48, 57, 
	97, 122, 48, 57, 9, 32, 47, 61, 
	48, 57, 9, 32, 47, 61, 62, 9, 
	10, 32, 40, 95, 48, 57, 65, 90, 
	97, 122, 34, 41, 43, 45, 48, 57, 
	97, 122, 34, 92, 9, 32, 41, 44, 
	9, 32, 44, 9, 32, 34, 43, 45, 
	48, 57, 48, 57, 9, 32, 41, 44, 
	46, 48, 57, 9, 10, 32, 48, 57, 
	9, 32, 41, 44, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 48, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 10, 9, 10, 32, 
	35, 65, 90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 4, 3, 1, 2, 0, 5, 1, 
	4, 2, 4, 3, 5, 0, 5, 3, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 8, 4, 1, 
	4, 0, 1, 0, 4, 3, 2, 2, 
	4, 2, 4, 3, 5, 0, 5, 3, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 4, 0, 1, 
	0, 4, 4, 2, 4, 3, 5, 0, 
	5, 1, 0, 4, 0, 2, 5, 2, 
	3, 3, 0, 0, 4, 0, 3, 2, 
	0, 4, 4, 1, 5, 4, 2, 4, 
	3, 5, 0, 5, 3, 0, 4, 0, 
	2, 5, 2, 3, 3, 0, 0, 4, 
	0, 3, 1, 4, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 0, 3, 1, 3, 0, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 3, 0, 0, 
	2, 1, 1, 1, 3, 1, 1, 3, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 2, 1, 1, 
	1, 3, 2, 0, 0, 0, 1, 1, 
	1, 0, 1, 1, 0, 3, 1, 0, 
	1, 1, 0, 1, 2, 1, 2, 2, 
	1, 1, 0, 0, 3, 2, 0, 0, 
	0, 1, 1, 1, 0, 1, 1, 0, 
	3, 1, 0, 1, 1, 0, 1, 2, 
	1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 7, 11, 13, 19, 21, 30, 
	32, 39, 42, 47, 51, 58, 60, 67, 
	71, 73, 79, 80, 86, 93, 96, 101, 
	106, 107, 109, 116, 118, 124, 136, 141, 
	143, 150, 152, 155, 157, 165, 170, 174, 
	180, 187, 190, 195, 199, 206, 208, 215, 
	219, 221, 227, 228, 234, 241, 244, 249, 
	254, 255, 257, 264, 266, 272, 279, 281, 
	284, 286, 294, 301, 304, 309, 313, 320, 
	322, 329, 331, 333, 339, 340, 346, 353, 
	356, 361, 366, 367, 369, 376, 378, 384, 
	389, 391, 397, 402, 404, 413, 420, 423, 
	428, 432, 439, 441, 448, 452, 454, 460, 
	461, 467, 474, 477, 482, 487, 488, 490, 
	497, 499, 505, 507, 514
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 5, 0, 1, 
	2, 1, 0, 2, 3, 6, 4, 4, 
	4, 4, 0, 7, 0, 8, 9, 8, 
	10, 7, 7, 7, 7, 0, 3, 0, 
	11, 12, 13, 13, 14, 15, 0, 17, 
	18, 16, 19, 19, 20, 21, 0, 22, 
	22, 23, 0, 23, 23, 11, 13, 13, 
	14, 0, 14, 0, 19, 19, 20, 21, 
	24, 14, 0, 25, 26, 25, 0, 27, 
	0, 19, 19, 20, 21, 27, 0, 16, 
	28, 15, 15, 15, 15, 0, 29, 29, 
	30, 31, 31, 32, 0, 34, 35, 33, 
	36, 36, 37, 38, 0, 39, 39, 12, 
	15, 0, 33, 32, 0, 36, 36, 37, 
	40, 32, 38, 0, 41, 0, 36, 36, 
	37, 41, 38, 0, 42, 43, 42, 44, 
	45, 46, 47, 5, 5, 5, 5, 0, 
	48, 48, 49, 50, 0, 51, 0, 51, 
	51, 52, 52, 53, 54, 0, 53, 0, 
	55, 53, 0, 54, 0, 56, 57, 56, 
	54, 54, 54, 54, 0, 58, 58, 59, 
	60, 0, 58, 58, 60, 0, 61, 60, 
	60, 60, 60, 0, 62, 63, 64, 64, 
	65, 66, 0, 68, 69, 67, 70, 70, 
	71, 72, 0, 73, 73, 74, 0, 74, 
	74, 62, 64, 64, 65, 0, 65, 0, 
	70, 70, 71, 72, 75, 65, 0, 76, 
	77, 76, 0, 78, 0, 70, 70, 71, 
	72, 78, 0, 67, 79, 66, 66, 66, 
	66, 0, 80, 80, 81, 82, 82, 83, 
	0, 85, 86, 84, 87, 87, 88, 89, 
	0, 90, 90, 63, 66, 0, 84, 83, 
	0, 87, 87, 88, 91, 83, 89, 0, 
	92, 0, 87, 87, 88, 92, 89, 0, 
	59, 59, 93, 93, 94, 95, 0, 94, 
	0, 96, 94, 0, 95, 0, 97, 98, 
	97, 95, 95, 95, 95, 0, 99, 100, 
	101, 101, 102, 103, 0, 105, 106, 104, 
	107, 107, 108, 109, 0, 110, 110, 111, 
	0, 111, 111, 99, 101, 101, 102, 0, 
	102, 0, 107, 107, 108, 109, 112, 102, 
	0, 113, 0, 114, 0, 107, 107, 108, 
	109, 114, 0, 104, 115, 103, 103, 103, 
	103, 0, 116, 116, 117, 118, 118, 119, 
	0, 121, 122, 120, 123, 123, 124, 125, 
	0, 126, 126, 100, 103, 0, 120, 119, 
	0, 123, 123, 124, 127, 119, 125, 0, 
	128, 0, 123, 123, 124, 128, 125, 0, 
	129, 129, 130, 131, 0, 130, 0, 132, 
	132, 133, 134, 130, 0, 135, 135, 49, 
	136, 0, 59, 0, 137, 138, 137, 139, 
	131, 131, 131, 131, 0, 140, 141, 142, 
	142, 143, 144, 0, 146, 147, 145, 148, 
	148, 149, 150, 0, 151, 151, 152, 0, 
	152, 152, 140, 142, 142, 143, 0, 143, 
	0, 148, 148, 149, 150, 153, 143, 0, 
	154, 155, 154, 0, 156, 0, 148, 148, 
	149, 150, 156, 0, 145, 157, 144, 144, 
	144, 144, 0, 158, 158, 159, 160, 160, 
	161, 0, 163, 164, 162, 165, 165, 166, 
	167, 0, 168, 168, 141, 144, 0, 162, 
	161, 0, 165, 165, 166, 169, 161, 167, 
	0, 170, 0, 165, 165, 166, 170, 167, 
	0, 172, 171, 1, 2, 1, 3, 4, 
	5, 0, 173, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 115, 3, 4, 29, 5, 6, 
	7, 115, 8, 9, 15, 13, 14, 19, 
	9, 10, 18, 11, 15, 12, 11, 12, 
	16, 7, 115, 17, 20, 20, 21, 25, 
	26, 21, 22, 24, 23, 15, 19, 23, 
	27, 28, 30, 115, 66, 87, 31, 37, 
	30, 31, 37, 32, 33, 34, 36, 35, 
	7, 115, 38, 61, 39, 40, 41, 47, 
	45, 46, 51, 41, 42, 50, 43, 47, 
	44, 43, 44, 48, 7, 115, 49, 52, 
	52, 53, 57, 58, 53, 54, 56, 55, 
	47, 51, 55, 59, 60, 62, 63, 65, 
	64, 7, 115, 67, 73, 71, 72, 77, 
	67, 68, 76, 69, 73, 70, 69, 70, 
	74, 115, 75, 78, 78, 79, 83, 84, 
	79, 80, 82, 81, 73, 77, 81, 85, 
	86, 88, 89, 92, 90, 31, 91, 90, 
	91, 7, 115, 93, 94, 100, 98, 99, 
	104, 94, 95, 103, 96, 100, 97, 96, 
	97, 101, 7, 115, 102, 105, 105, 106, 
	110, 111, 106, 107, 109, 108, 100, 104, 
	108, 112, 113, 114, 116, 0
};

static const char _command_trans_actions_wi[] = {
	21, 0, 19, 0, 1, 1, 7, 1, 
	37, 70, 3, 0, 0, 1, 1, 1, 
	1, 0, 1, 40, 40, 40, 0, 0, 
	1, 17, 52, 1, 5, 0, 0, 1, 
	1, 1, 0, 1, 43, 43, 78, 0, 
	1, 1, 25, 74, 3, 25, 25, 25, 
	0, 0, 0, 0, 1, 1, 1, 11, 
	31, 62, 0, 0, 1, 7, 0, 0, 
	1, 1, 1, 1, 0, 1, 40, 40, 
	40, 0, 0, 1, 13, 46, 1, 5, 
	0, 0, 1, 1, 1, 0, 1, 43, 
	43, 78, 0, 1, 1, 1, 1, 1, 
	11, 28, 58, 0, 0, 1, 1, 1, 
	1, 0, 1, 40, 40, 40, 0, 0, 
	1, 55, 1, 5, 0, 0, 1, 1, 
	1, 0, 1, 43, 43, 78, 0, 1, 
	1, 1, 1, 1, 9, 9, 9, 0, 
	0, 34, 66, 3, 0, 0, 1, 1, 
	1, 1, 0, 1, 40, 40, 40, 0, 
	0, 1, 15, 49, 1, 5, 0, 0, 
	1, 1, 1, 0, 1, 43, 43, 78, 
	0, 1, 1, 0, 23, 0
};

static const int command_start = 1;
static const int command_first_final = 115;

static const int command_en_eat_line = 114;
static const int command_en_main = 1;

#line 11 "classes/command.rl"


void Command::initialize()
{
  int cs;
  
  mAction = NO_ACTION;
  mServer = NULL;
  mQuit   = false;
  mTokenIndex = 0;
  mSilent = false;
  mThread = NULL;
  
  clear();
  
  
#line 307 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 27 "classes/command.rl"
  mCurrentState = cs;
}

int Command::do_listen()
{
  char buffer[1024];
  char * line = buffer;
  
  // set thread priority to normal
  mServer->normal_priority();
  
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
  int cs = mCurrentState;        // restore machine state
  
  
#line 344 "classes/command.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _out;
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
	cs = _command_trans_targs_wi[_trans];

	if ( _command_trans_actions_wi[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions_wi[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 59 "classes/command.rl"
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
#line 72 "classes/command.rl"
	{ set_from_token(mVariable);}
	break;
	case 2:
#line 74 "classes/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 76 "classes/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 78 "classes/command.rl"
	{ set_from_token(mClass);}
	break;
	case 5:
#line 80 "classes/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 82 "classes/command.rl"
	{ mFrom = mVariable; }
	break;
	case 7:
#line 84 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 89 "classes/command.rl"
	{
      set_from_token(mValue);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 9:
#line 94 "classes/command.rl"
	{ 
      add_value_from_token(); 
    }
	break;
	case 10:
#line 98 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 100 "classes/command.rl"
	{
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 105 "classes/command.rl"
	{
      mTo   = mVariable;
      remove_link();
    }
	break;
	case 13:
#line 110 "classes/command.rl"
	{ create_instance(); }
	break;
	case 14:
#line 112 "classes/command.rl"
	{ execute_method(); }
	break;
	case 15:
#line 114 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 16:
#line 116 "classes/command.rl"
	{ execute_command(); }
	break;
	case 17:
#line 120 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
      }
    }
	break;
	case 18:
#line 125 "classes/command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 114; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 19:
#line 134 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 534 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 184 "classes/command.rl"

//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close()
{
  mQuit = true;
  if (mThread)
    pthread_join( mThread, NULL); // wait for child to finish
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
#ifdef DEBUG_PARSER
  if (&pElem == &mValue) std::cout <<    "[val " << mToken << "]" << std::endl;
  if (&pElem == &mVariable) std::cout << "[var " << mToken << "]" << std::endl;
  if (&pElem == &mClass) std::cout <<    "[cla " << mToken << "]" << std::endl;
#endif
  pElem = mToken;
  mTokenIndex = 0;
}

void Command::add_value_from_token () 
{
  mParameters.add(mValue); // add to 'list' value
}

void Command::set_parameter  (const std::string& pKey, const std::string& pValue) 
{
  mParameters.set(pKey,pValue);
}

// FIXME: create_instance should run in server space with concurrency locks.
void Command::create_instance()
{
  mServer->lock();
  Node * node = mServer->create_instance(mVariable, mClass, mParameters, mOutput);
#ifdef DEBUG_PARSER
  std::cout << "NEW "<< mVariable << " = " << mClass << "(" << mParameters << ")";
#endif
  if (node) {
    if (!mSilent)
      *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
  mServer->unlock();
}


void Command::create_link()
{ 
  mServer->lock();
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

void Command::remove_link()
{ 
  mServer->lock();
  mServer->remove_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "UNLINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mVariable)) {
    node->execute_method(mMethod, mParameters);
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_class_method()
{
  Class * klass;
  mServer->lock();
  if (Class::get(&klass, mClass)) {
    klass->execute_method(mMethod, mParameters, mOutput);
  } else {
    *mOutput << "Unknown class '" << mClass << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_command()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
    
  } else if (mMethod == "quit" || mMethod == "q") {
    mServer->quit();
    mQuit = true;
    *mOutput << "Bye..." << std::endl;
    
  } else if (mMethod == "set_lib_path") {
    std::string path;
    if (!mParameters.get(&path)) {
      *mOutput << "Could not set library path (no parameter).\n";
      return;
    }
    Class::set_lib_path(path);
    *mOutput << "Library path set to '" << path << "'.\n";
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::clear() 
{
  mTokenIndex = 0;
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mParameters.clear();
  mFromPort = 1;
  mToPort   = 1;
}
