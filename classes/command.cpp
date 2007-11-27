#line 1 "classes/command.rl"
#include "command.h"
#include "rubyk.h"
#include "class.h"
#include "mutex.h"

#undef DEBUG_PARSER


#line 11 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 7, 1, 8, 1, 12, 1, 
	13, 1, 14, 1, 16, 1, 17, 1, 
	18, 2, 1, 6, 2, 1, 11, 2, 
	2, 13, 2, 2, 14, 2, 5, 9, 
	2, 5, 10, 2, 12, 16, 2, 13, 
	16, 2, 14, 16, 2, 15, 16, 3, 
	1, 11, 16, 3, 2, 13, 16, 3, 
	2, 14, 16, 3, 2, 15, 16, 3, 
	5, 10, 0
};

static const short _command_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 22, 33, 
	34, 42, 44, 48, 51, 58, 60, 67, 
	70, 72, 78, 78, 86, 93, 95, 100, 
	105, 105, 107, 115, 117, 124, 137, 140, 
	145, 149, 157, 165, 167, 171, 174, 181, 
	183, 190, 193, 195, 201, 201, 209, 216, 
	218, 223, 228, 228, 230, 238, 240, 247, 
	255, 257, 260, 262, 272, 280, 282, 286, 
	289, 296, 298, 305, 306, 308, 314, 314, 
	322, 329, 331, 336, 341, 341, 343, 351, 
	353, 360, 366, 368, 373, 376, 377, 388, 
	396, 398, 402, 405, 412, 414, 421, 424, 
	426, 432, 432, 440, 447, 449, 454, 459, 
	459, 461, 469, 471, 478, 479, 487
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
	46, 61, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 61, 9, 32, 62, 65, 
	90, 9, 32, 65, 90, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 43, 45, 48, 57, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 9, 10, 
	32, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 34, 43, 45, 48, 57, 
	34, 92, 9, 32, 41, 97, 122, 9, 
	32, 41, 97, 122, 48, 57, 9, 32, 
	41, 46, 48, 57, 97, 122, 48, 57, 
	9, 32, 41, 48, 57, 97, 122, 9, 
	32, 43, 45, 48, 57, 97, 122, 48, 
	57, 46, 48, 57, 97, 122, 9, 10, 
	32, 95, 48, 57, 65, 90, 97, 122, 
	34, 41, 43, 45, 48, 57, 97, 122, 
	34, 92, 9, 32, 41, 44, 9, 32, 
	44, 9, 32, 34, 43, 45, 48, 57, 
	48, 57, 9, 32, 41, 44, 46, 48, 
	57, 10, 48, 57, 9, 32, 41, 44, 
	48, 57, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 34, 43, 45, 48, 
	57, 34, 92, 9, 32, 41, 97, 122, 
	9, 32, 41, 97, 122, 48, 57, 9, 
	32, 41, 46, 48, 57, 97, 122, 48, 
	57, 9, 32, 41, 48, 57, 97, 122, 
	43, 45, 48, 57, 97, 122, 48, 57, 
	9, 32, 61, 48, 57, 9, 32, 61, 
	62, 9, 10, 32, 40, 95, 48, 57, 
	65, 90, 97, 122, 34, 41, 43, 45, 
	48, 57, 97, 122, 34, 92, 9, 32, 
	41, 44, 9, 32, 44, 9, 32, 34, 
	43, 45, 48, 57, 48, 57, 9, 32, 
	41, 44, 46, 48, 57, 9, 10, 32, 
	48, 57, 9, 32, 41, 44, 48, 57, 
	58, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 34, 43, 45, 48, 57, 34, 
	92, 9, 32, 41, 97, 122, 9, 32, 
	41, 97, 122, 48, 57, 9, 32, 41, 
	46, 48, 57, 97, 122, 48, 57, 9, 
	32, 41, 48, 57, 97, 122, 10, 9, 
	10, 32, 35, 65, 90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 4, 3, 1, 2, 0, 5, 1, 
	4, 2, 4, 3, 5, 0, 5, 3, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 7, 3, 3, 
	2, 2, 4, 2, 4, 3, 5, 0, 
	5, 3, 0, 4, 0, 2, 5, 2, 
	3, 3, 0, 0, 4, 0, 3, 4, 
	0, 1, 0, 4, 4, 2, 4, 3, 
	5, 0, 5, 1, 0, 4, 0, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 2, 0, 3, 3, 1, 5, 4, 
	2, 4, 3, 5, 0, 5, 3, 0, 
	4, 0, 2, 5, 2, 3, 3, 0, 
	0, 4, 0, 3, 1, 4, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 0, 3, 1, 3, 0, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 3, 0, 1, 
	1, 3, 2, 0, 0, 0, 1, 1, 
	1, 0, 1, 1, 0, 3, 1, 0, 
	1, 1, 0, 1, 2, 1, 2, 2, 
	1, 1, 1, 3, 2, 0, 0, 0, 
	1, 1, 1, 0, 1, 1, 0, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 2, 1, 1, 0, 0, 3, 2, 
	0, 0, 0, 1, 1, 1, 0, 1, 
	1, 0, 3, 1, 0, 1, 1, 0, 
	1, 2, 1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 7, 11, 13, 19, 21, 30, 
	32, 39, 42, 47, 51, 58, 60, 67, 
	71, 73, 79, 80, 86, 93, 96, 101, 
	106, 107, 109, 116, 118, 124, 135, 139, 
	144, 148, 154, 161, 164, 169, 173, 180, 
	182, 189, 193, 195, 201, 202, 208, 215, 
	218, 223, 228, 229, 231, 238, 240, 246, 
	253, 255, 258, 260, 268, 275, 278, 283, 
	287, 294, 296, 303, 305, 307, 313, 314, 
	320, 327, 330, 335, 340, 341, 343, 350, 
	352, 358, 363, 365, 370, 374, 376, 385, 
	392, 395, 400, 404, 411, 413, 420, 424, 
	426, 432, 433, 439, 446, 449, 454, 459, 
	460, 462, 469, 471, 477, 479, 486
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
	45, 46, 5, 5, 5, 5, 0, 47, 
	47, 48, 0, 49, 49, 50, 51, 0, 
	49, 49, 51, 0, 52, 51, 51, 51, 
	51, 0, 53, 54, 55, 55, 56, 57, 
	0, 59, 60, 58, 61, 61, 62, 63, 
	0, 64, 64, 65, 0, 65, 65, 53, 
	55, 55, 56, 0, 56, 0, 61, 61, 
	62, 63, 66, 56, 0, 67, 68, 67, 
	0, 69, 0, 61, 61, 62, 63, 69, 
	0, 58, 70, 57, 57, 57, 57, 0, 
	71, 71, 72, 73, 73, 74, 0, 76, 
	77, 75, 78, 78, 79, 80, 0, 81, 
	81, 54, 57, 0, 75, 74, 0, 78, 
	78, 79, 82, 74, 80, 0, 83, 0, 
	78, 78, 79, 83, 80, 0, 50, 50, 
	84, 84, 85, 86, 0, 85, 0, 87, 
	85, 0, 86, 0, 88, 89, 88, 86, 
	86, 86, 86, 0, 90, 91, 92, 92, 
	93, 94, 0, 96, 97, 95, 98, 98, 
	99, 100, 0, 101, 101, 102, 0, 102, 
	102, 90, 92, 92, 93, 0, 93, 0, 
	98, 98, 99, 100, 103, 93, 0, 104, 
	0, 105, 0, 98, 98, 99, 100, 105, 
	0, 95, 106, 94, 94, 94, 94, 0, 
	107, 107, 108, 109, 109, 110, 0, 112, 
	113, 111, 114, 114, 115, 116, 0, 117, 
	117, 91, 94, 0, 111, 110, 0, 114, 
	114, 115, 118, 110, 116, 0, 119, 0, 
	114, 114, 115, 119, 116, 0, 120, 120, 
	121, 122, 0, 121, 0, 123, 123, 124, 
	121, 0, 125, 125, 126, 0, 50, 0, 
	127, 128, 127, 129, 122, 122, 122, 122, 
	0, 130, 131, 132, 132, 133, 134, 0, 
	136, 137, 135, 138, 138, 139, 140, 0, 
	141, 141, 142, 0, 142, 142, 130, 132, 
	132, 133, 0, 133, 0, 138, 138, 139, 
	140, 143, 133, 0, 144, 145, 144, 0, 
	146, 0, 138, 138, 139, 140, 146, 0, 
	135, 147, 134, 134, 134, 134, 0, 148, 
	148, 149, 150, 150, 151, 0, 153, 154, 
	152, 155, 155, 156, 157, 0, 158, 158, 
	131, 134, 0, 152, 151, 0, 155, 155, 
	156, 159, 151, 157, 0, 160, 0, 155, 
	155, 156, 160, 157, 0, 162, 161, 1, 
	2, 1, 3, 4, 5, 0, 163, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 109, 3, 4, 29, 5, 6, 
	7, 109, 8, 9, 15, 13, 14, 19, 
	9, 10, 18, 11, 15, 12, 11, 12, 
	16, 7, 109, 17, 20, 20, 21, 25, 
	26, 21, 22, 24, 23, 15, 19, 23, 
	27, 28, 30, 109, 60, 81, 31, 30, 
	31, 32, 55, 33, 34, 35, 41, 39, 
	40, 45, 35, 36, 44, 37, 41, 38, 
	37, 38, 42, 7, 109, 43, 46, 46, 
	47, 51, 52, 47, 48, 50, 49, 41, 
	45, 49, 53, 54, 56, 57, 59, 58, 
	7, 109, 61, 67, 65, 66, 71, 61, 
	62, 70, 63, 67, 64, 63, 64, 68, 
	109, 69, 72, 72, 73, 77, 78, 73, 
	74, 76, 75, 67, 71, 75, 79, 80, 
	82, 83, 86, 84, 85, 84, 85, 7, 
	109, 87, 88, 94, 92, 93, 98, 88, 
	89, 97, 90, 94, 91, 90, 91, 95, 
	7, 109, 96, 99, 99, 100, 104, 105, 
	100, 101, 103, 102, 94, 98, 102, 106, 
	107, 108, 110, 0
};

static const char _command_trans_actions_wi[] = {
	21, 0, 19, 0, 1, 1, 7, 1, 
	34, 63, 3, 0, 0, 1, 1, 1, 
	1, 0, 1, 37, 37, 37, 0, 0, 
	1, 17, 49, 1, 5, 0, 0, 1, 
	1, 1, 0, 1, 40, 40, 71, 0, 
	1, 1, 25, 67, 3, 25, 25, 0, 
	0, 0, 0, 1, 7, 0, 0, 1, 
	1, 1, 1, 0, 1, 37, 37, 37, 
	0, 0, 1, 13, 43, 1, 5, 0, 
	0, 1, 1, 1, 0, 1, 40, 40, 
	71, 0, 1, 1, 1, 1, 1, 11, 
	28, 55, 0, 0, 1, 1, 1, 1, 
	0, 1, 37, 37, 37, 0, 0, 1, 
	52, 1, 5, 0, 0, 1, 1, 1, 
	0, 1, 40, 40, 71, 0, 1, 1, 
	1, 1, 1, 9, 9, 0, 0, 31, 
	59, 3, 0, 0, 1, 1, 1, 1, 
	0, 1, 37, 37, 37, 0, 0, 1, 
	15, 46, 1, 5, 0, 0, 1, 1, 
	1, 0, 1, 40, 40, 71, 0, 1, 
	1, 0, 23, 0
};

static const int command_start = 1;
static const int command_first_final = 109;

static const int command_en_eat_line = 108;
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
  
  
#line 292 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 27 "classes/command.rl"
  mCurrentState = cs;
}

int Command::do_listen()
{
  char iss[1024];
  
  // set thread priority to normal
  mServer->normal_priority();
  
  if (!mSilent)
    *mOutput << "Welcome to rubyk !\n\n";
  clear();
  prompt();

  while(!mQuit && !mInput->eof()) {
    mInput->getline(iss,1023); // '\n'
    parse(iss);
    parse("\n");
  }
  return 0; // thread return value
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs = mCurrentState;        // restore machine state
  
  
#line 327 "classes/command.cpp"
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
#line 57 "classes/command.rl"
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
#line 70 "classes/command.rl"
	{ set_from_token(mVariable);}
	break;
	case 2:
#line 72 "classes/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 74 "classes/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 76 "classes/command.rl"
	{ set_from_token(mClass);}
	break;
	case 5:
#line 78 "classes/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 80 "classes/command.rl"
	{ mFrom = mVariable; }
	break;
	case 7:
#line 82 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 87 "classes/command.rl"
	{
      set_from_token(mValue);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 9:
#line 92 "classes/command.rl"
	{ 
      add_value_from_token(); 
    }
	break;
	case 10:
#line 96 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 98 "classes/command.rl"
	{
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 103 "classes/command.rl"
	{ create_instance(); }
	break;
	case 13:
#line 105 "classes/command.rl"
	{ execute_method(); }
	break;
	case 14:
#line 107 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 15:
#line 109 "classes/command.rl"
	{ execute_command(); }
	break;
	case 16:
#line 113 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
	case 17:
#line 119 "classes/command.rl"
	{
      p--; // move back one char
      *mOutput << "Syntax error !" << std::endl;
      clear();
      prompt();
      {cs = 108; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 18:
#line 127 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 510 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 172 "classes/command.rl"

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

  // if (!mSilent)  // useless
  //   *mOutput << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mVariable)) {
    node->execute_method(mMethod, mParameters); 
    if (!mSilent && mMethod == "bang") *mOutput << node->inspect() << std::endl;
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
  } else if (mMethod == "quit") {
    mServer->quit();
    mQuit = true;
    *mOutput << "Bye..." << std::endl;
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::print(void)
{
  switch(mAction) {
  case CREATE_INSTANCE:
    *mOutput << mVariable << " = " << mClass << "(" << mParameters << ")" << std::endl;
    break;
  case CREATE_LINK:
    *mOutput << "[todo]" << std::endl;
    break;
  }
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
