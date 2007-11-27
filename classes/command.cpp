#line 1 "classes/command.rl"
#include "command.h"
#include "rubyk.h"
#include "class.h"
#include "mutex.h"

#undef DEBUG_PARSER


#line 11 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 7, 1, 8, 1, 16, 1, 
	17, 1, 18, 2, 1, 6, 2, 5, 
	9, 2, 5, 10, 2, 12, 16, 2, 
	13, 16, 2, 14, 16, 2, 15, 16, 
	3, 1, 11, 16, 3, 2, 13, 16, 
	3, 2, 14, 16, 3, 2, 15, 16, 
	3, 5, 10, 0
};

static const short _command_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 22, 31, 
	39, 41, 45, 48, 55, 57, 64, 65, 
	67, 73, 73, 81, 88, 90, 95, 100, 
	100, 102, 110, 112, 119, 132, 135, 140, 
	144, 152, 160, 162, 166, 169, 176, 178, 
	185, 186, 188, 194, 194, 202, 209, 211, 
	216, 221, 221, 223, 231, 233, 240, 248, 
	250, 253, 255, 263, 271, 273, 277, 280, 
	287, 289, 296, 297, 299, 305, 305, 313, 
	320, 322, 327, 332, 332, 334, 342, 344, 
	351, 357, 359, 364, 367, 368, 377, 385, 
	387, 391, 394, 401, 403, 410, 411, 413, 
	419, 419, 427, 434, 436, 441, 446, 446, 
	448, 456, 458, 465, 466, 474
};

static const char _command_trans_keys[] = {
	9, 10, 32, 35, 65, 90, 97, 122, 
	9, 10, 32, 10, 46, 95, 48, 57, 
	65, 90, 97, 122, 97, 122, 10, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	41, 43, 45, 48, 57, 97, 122, 34, 
	92, 9, 32, 41, 44, 9, 32, 44, 
	9, 32, 34, 43, 45, 48, 57, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	10, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 34, 43, 45, 48, 57, 
	34, 92, 9, 32, 41, 97, 122, 9, 
	32, 41, 97, 122, 48, 57, 9, 32, 
	41, 46, 48, 57, 97, 122, 48, 57, 
	9, 32, 41, 48, 57, 97, 122, 9, 
	10, 32, 40, 46, 61, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 61, 9, 
	32, 62, 65, 90, 9, 32, 65, 90, 
	40, 95, 48, 57, 65, 90, 97, 122, 
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
	9, 32, 43, 45, 48, 57, 97, 122, 
	48, 57, 46, 48, 57, 97, 122, 10, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	41, 43, 45, 48, 57, 97, 122, 34, 
	92, 9, 32, 41, 44, 9, 32, 44, 
	9, 32, 34, 43, 45, 48, 57, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	10, 48, 57, 9, 32, 41, 44, 48, 
	57, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 34, 43, 45, 48, 57, 
	34, 92, 9, 32, 41, 97, 122, 9, 
	32, 41, 97, 122, 48, 57, 9, 32, 
	41, 46, 48, 57, 97, 122, 48, 57, 
	9, 32, 41, 48, 57, 97, 122, 43, 
	45, 48, 57, 97, 122, 48, 57, 9, 
	32, 61, 48, 57, 9, 32, 61, 62, 
	10, 40, 95, 48, 57, 65, 90, 97, 
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
	122, 10, 9, 10, 32, 35, 65, 90, 
	97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 4, 3, 1, 2, 0, 3, 4, 
	2, 4, 3, 5, 0, 5, 1, 0, 
	4, 0, 2, 5, 2, 3, 3, 0, 
	0, 4, 0, 3, 7, 3, 3, 2, 
	2, 4, 2, 4, 3, 5, 0, 5, 
	1, 0, 4, 0, 2, 5, 2, 3, 
	3, 0, 0, 4, 0, 3, 4, 0, 
	1, 0, 2, 4, 2, 4, 3, 5, 
	0, 5, 1, 0, 4, 0, 2, 5, 
	2, 3, 3, 0, 0, 4, 0, 3, 
	2, 0, 3, 3, 1, 3, 4, 2, 
	4, 3, 5, 0, 5, 1, 0, 4, 
	0, 2, 5, 2, 3, 3, 0, 0, 
	4, 0, 3, 1, 4, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 0, 3, 1, 3, 2, 
	0, 0, 0, 1, 1, 1, 0, 1, 
	1, 0, 3, 1, 0, 1, 1, 0, 
	1, 2, 1, 2, 3, 0, 1, 1, 
	3, 2, 0, 0, 0, 1, 1, 1, 
	0, 1, 1, 0, 3, 1, 0, 1, 
	1, 0, 1, 2, 1, 2, 2, 1, 
	1, 1, 3, 2, 0, 0, 0, 1, 
	1, 1, 0, 1, 1, 0, 3, 1, 
	0, 1, 1, 0, 1, 2, 1, 2, 
	2, 1, 1, 0, 0, 3, 2, 0, 
	0, 0, 1, 1, 1, 0, 1, 1, 
	0, 3, 1, 0, 1, 1, 0, 1, 
	2, 1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 7, 11, 13, 19, 21, 28, 
	35, 38, 43, 47, 54, 56, 63, 65, 
	67, 73, 74, 80, 87, 90, 95, 100, 
	101, 103, 110, 112, 118, 129, 133, 138, 
	142, 148, 155, 158, 163, 167, 174, 176, 
	183, 185, 187, 193, 194, 200, 207, 210, 
	215, 220, 221, 223, 230, 232, 238, 245, 
	247, 250, 252, 258, 265, 268, 273, 277, 
	284, 286, 293, 295, 297, 303, 304, 310, 
	317, 320, 325, 330, 331, 333, 340, 342, 
	348, 353, 355, 360, 364, 366, 373, 380, 
	383, 388, 392, 399, 401, 408, 410, 412, 
	418, 419, 425, 432, 435, 440, 445, 446, 
	448, 455, 457, 463, 465, 472
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 5, 0, 1, 
	2, 1, 0, 2, 3, 6, 4, 4, 
	4, 4, 0, 7, 0, 8, 9, 7, 
	7, 7, 7, 0, 10, 11, 12, 12, 
	13, 14, 0, 16, 17, 15, 18, 18, 
	19, 20, 0, 21, 21, 22, 0, 22, 
	22, 10, 12, 12, 13, 0, 13, 0, 
	18, 18, 19, 20, 23, 13, 0, 24, 
	0, 25, 0, 18, 18, 19, 20, 25, 
	0, 15, 26, 14, 14, 14, 14, 0, 
	27, 27, 28, 29, 29, 30, 0, 32, 
	33, 31, 34, 34, 35, 36, 0, 37, 
	37, 11, 14, 0, 31, 30, 0, 34, 
	34, 35, 38, 30, 36, 0, 39, 0, 
	34, 34, 35, 39, 36, 0, 40, 41, 
	40, 42, 43, 44, 5, 5, 5, 5, 
	0, 45, 45, 46, 0, 47, 47, 48, 
	49, 0, 47, 47, 49, 0, 50, 49, 
	49, 49, 49, 0, 51, 52, 53, 53, 
	54, 55, 0, 57, 58, 56, 59, 59, 
	60, 61, 0, 62, 62, 63, 0, 63, 
	63, 51, 53, 53, 54, 0, 54, 0, 
	59, 59, 60, 61, 64, 54, 0, 65, 
	0, 66, 0, 59, 59, 60, 61, 66, 
	0, 56, 67, 55, 55, 55, 55, 0, 
	68, 68, 69, 70, 70, 71, 0, 73, 
	74, 72, 75, 75, 76, 77, 0, 78, 
	78, 52, 55, 0, 72, 71, 0, 75, 
	75, 76, 79, 71, 77, 0, 80, 0, 
	75, 75, 76, 80, 77, 0, 48, 48, 
	81, 81, 82, 83, 0, 82, 0, 84, 
	82, 0, 83, 0, 85, 83, 83, 83, 
	83, 0, 86, 87, 88, 88, 89, 90, 
	0, 92, 93, 91, 94, 94, 95, 96, 
	0, 97, 97, 98, 0, 98, 98, 86, 
	88, 88, 89, 0, 89, 0, 94, 94, 
	95, 96, 99, 89, 0, 100, 0, 101, 
	0, 94, 94, 95, 96, 101, 0, 91, 
	102, 90, 90, 90, 90, 0, 103, 103, 
	104, 105, 105, 106, 0, 108, 109, 107, 
	110, 110, 111, 112, 0, 113, 113, 87, 
	90, 0, 107, 106, 0, 110, 110, 111, 
	114, 106, 112, 0, 115, 0, 110, 110, 
	111, 115, 112, 0, 116, 116, 117, 118, 
	0, 117, 0, 119, 119, 120, 117, 0, 
	121, 121, 122, 0, 48, 0, 123, 124, 
	118, 118, 118, 118, 0, 125, 126, 127, 
	127, 128, 129, 0, 131, 132, 130, 133, 
	133, 134, 135, 0, 136, 136, 137, 0, 
	137, 137, 125, 127, 127, 128, 0, 128, 
	0, 133, 133, 134, 135, 138, 128, 0, 
	139, 0, 140, 0, 133, 133, 134, 135, 
	140, 0, 130, 141, 129, 129, 129, 129, 
	0, 142, 142, 143, 144, 144, 145, 0, 
	147, 148, 146, 149, 149, 150, 151, 0, 
	152, 152, 126, 129, 0, 146, 145, 0, 
	149, 149, 150, 153, 145, 151, 0, 154, 
	0, 149, 149, 150, 154, 151, 0, 156, 
	155, 1, 2, 1, 3, 4, 5, 0, 
	157, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 108, 3, 4, 28, 5, 6, 
	108, 7, 8, 14, 12, 13, 18, 8, 
	9, 17, 10, 14, 11, 10, 11, 15, 
	108, 16, 19, 19, 20, 24, 25, 20, 
	21, 23, 22, 14, 18, 22, 26, 27, 
	29, 108, 59, 80, 30, 29, 30, 31, 
	54, 32, 33, 34, 40, 38, 39, 44, 
	34, 35, 43, 36, 40, 37, 36, 37, 
	41, 108, 42, 45, 45, 46, 50, 51, 
	46, 47, 49, 48, 40, 44, 48, 52, 
	53, 55, 56, 58, 57, 108, 60, 66, 
	64, 65, 70, 60, 61, 69, 62, 66, 
	63, 62, 63, 67, 108, 68, 71, 71, 
	72, 76, 77, 72, 73, 75, 74, 66, 
	70, 74, 78, 79, 81, 82, 85, 83, 
	84, 83, 84, 108, 86, 87, 93, 91, 
	92, 97, 87, 88, 96, 89, 93, 90, 
	89, 90, 94, 108, 95, 98, 98, 99, 
	103, 104, 99, 100, 102, 101, 93, 97, 
	101, 105, 106, 107, 109, 0
};

static const char _command_trans_actions_wi[] = {
	15, 0, 13, 0, 1, 1, 7, 1, 
	48, 3, 0, 0, 1, 1, 1, 1, 
	0, 1, 22, 22, 22, 0, 0, 1, 
	34, 1, 5, 0, 0, 1, 1, 1, 
	0, 1, 25, 25, 56, 0, 1, 1, 
	19, 52, 3, 19, 19, 0, 0, 0, 
	0, 1, 7, 0, 0, 1, 1, 1, 
	1, 0, 1, 22, 22, 22, 0, 0, 
	1, 28, 1, 5, 0, 0, 1, 1, 
	1, 0, 1, 25, 25, 56, 0, 1, 
	1, 1, 1, 1, 11, 40, 0, 0, 
	1, 1, 1, 1, 0, 1, 22, 22, 
	22, 0, 0, 1, 37, 1, 5, 0, 
	0, 1, 1, 1, 0, 1, 25, 25, 
	56, 0, 1, 1, 1, 1, 1, 9, 
	9, 0, 0, 44, 3, 0, 0, 1, 
	1, 1, 1, 0, 1, 22, 22, 22, 
	0, 0, 1, 31, 1, 5, 0, 0, 
	1, 1, 1, 0, 1, 25, 25, 56, 
	0, 1, 1, 0, 17, 0
};

static const int command_start = 1;
static const int command_first_final = 108;

static const int command_en_eat_line = 107;
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
  
  
#line 286 "classes/command.cpp"
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
  
  
#line 321 "classes/command.cpp"
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
      {cs = 107; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 18:
#line 127 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 504 "classes/command.cpp"
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
