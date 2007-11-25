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
	0, 0, 7, 10, 18, 20, 29, 37, 
	39, 43, 46, 53, 55, 62, 63, 65, 
	71, 71, 79, 86, 88, 93, 98, 98, 
	100, 108, 110, 117, 130, 133, 138, 142, 
	150, 158, 160, 164, 167, 174, 176, 183, 
	184, 186, 192, 192, 200, 207, 209, 214, 
	219, 219, 221, 229, 231, 238, 246, 248, 
	251, 253, 261, 269, 271, 275, 278, 285, 
	287, 294, 295, 297, 303, 303, 311, 318, 
	320, 325, 330, 330, 332, 340, 342, 349, 
	355, 357, 362, 365, 366, 375, 383, 385, 
	389, 392, 399, 401, 408, 409, 411, 417, 
	417, 425, 432, 434, 439, 444, 444, 446, 
	454, 456, 463, 464, 471
};

static const char _command_trans_keys[] = {
	9, 10, 32, 65, 90, 97, 122, 9, 
	10, 32, 46, 95, 48, 57, 65, 90, 
	97, 122, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 43, 45, 48, 57, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 10, 48, 
	57, 9, 32, 41, 44, 48, 57, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 34, 43, 45, 48, 57, 34, 92, 
	9, 32, 41, 97, 122, 9, 32, 41, 
	97, 122, 48, 57, 9, 32, 41, 46, 
	48, 57, 97, 122, 48, 57, 9, 32, 
	41, 48, 57, 97, 122, 9, 10, 32, 
	40, 46, 61, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 61, 9, 32, 62, 
	65, 90, 9, 32, 65, 90, 40, 95, 
	48, 57, 65, 90, 97, 122, 34, 41, 
	43, 45, 48, 57, 97, 122, 34, 92, 
	9, 32, 41, 44, 9, 32, 44, 9, 
	32, 34, 43, 45, 48, 57, 48, 57, 
	9, 32, 41, 44, 46, 48, 57, 10, 
	48, 57, 9, 32, 41, 44, 48, 57, 
	58, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 34, 43, 45, 48, 57, 34, 
	92, 9, 32, 41, 97, 122, 9, 32, 
	41, 97, 122, 48, 57, 9, 32, 41, 
	46, 48, 57, 97, 122, 48, 57, 9, 
	32, 41, 48, 57, 97, 122, 9, 32, 
	43, 45, 48, 57, 97, 122, 48, 57, 
	46, 48, 57, 97, 122, 10, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 43, 45, 48, 57, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 10, 48, 
	57, 9, 32, 41, 44, 48, 57, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 34, 43, 45, 48, 57, 34, 92, 
	9, 32, 41, 97, 122, 9, 32, 41, 
	97, 122, 48, 57, 9, 32, 41, 46, 
	48, 57, 97, 122, 48, 57, 9, 32, 
	41, 48, 57, 97, 122, 43, 45, 48, 
	57, 97, 122, 48, 57, 9, 32, 61, 
	48, 57, 9, 32, 61, 62, 10, 40, 
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
	9, 32, 41, 48, 57, 97, 122, 10, 
	9, 10, 32, 65, 90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 2, 0, 3, 4, 2, 
	4, 3, 5, 0, 5, 1, 0, 4, 
	0, 2, 5, 2, 3, 3, 0, 0, 
	4, 0, 3, 7, 3, 3, 2, 2, 
	4, 2, 4, 3, 5, 0, 5, 1, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 4, 0, 1, 
	0, 2, 4, 2, 4, 3, 5, 0, 
	5, 1, 0, 4, 0, 2, 5, 2, 
	3, 3, 0, 0, 4, 0, 3, 2, 
	0, 3, 3, 1, 3, 4, 2, 4, 
	3, 5, 0, 5, 1, 0, 4, 0, 
	2, 5, 2, 3, 3, 0, 0, 4, 
	0, 3, 1, 3, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 3, 1, 3, 2, 0, 
	0, 0, 1, 1, 1, 0, 1, 1, 
	0, 3, 1, 0, 1, 1, 0, 1, 
	2, 1, 2, 3, 0, 1, 1, 3, 
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
	0, 0, 6, 10, 16, 18, 25, 32, 
	35, 40, 44, 51, 53, 60, 62, 64, 
	70, 71, 77, 84, 87, 92, 97, 98, 
	100, 107, 109, 115, 126, 130, 135, 139, 
	145, 152, 155, 160, 164, 171, 173, 180, 
	182, 184, 190, 191, 197, 204, 207, 212, 
	217, 218, 220, 227, 229, 235, 242, 244, 
	247, 249, 255, 262, 265, 270, 274, 281, 
	283, 290, 292, 294, 300, 301, 307, 314, 
	317, 322, 327, 328, 330, 337, 339, 345, 
	350, 352, 357, 361, 363, 370, 377, 380, 
	385, 389, 396, 398, 405, 407, 409, 415, 
	416, 422, 429, 432, 437, 442, 443, 445, 
	452, 454, 460, 462, 468
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 0, 1, 2, 
	1, 0, 5, 3, 3, 3, 3, 0, 
	6, 0, 7, 8, 6, 6, 6, 6, 
	0, 9, 10, 11, 11, 12, 13, 0, 
	15, 16, 14, 17, 17, 18, 19, 0, 
	20, 20, 21, 0, 21, 21, 9, 11, 
	11, 12, 0, 12, 0, 17, 17, 18, 
	19, 22, 12, 0, 23, 0, 24, 0, 
	17, 17, 18, 19, 24, 0, 14, 25, 
	13, 13, 13, 13, 0, 26, 26, 27, 
	28, 28, 29, 0, 31, 32, 30, 33, 
	33, 34, 35, 0, 36, 36, 10, 13, 
	0, 30, 29, 0, 33, 33, 34, 37, 
	29, 35, 0, 38, 0, 33, 33, 34, 
	38, 35, 0, 39, 40, 39, 41, 42, 
	43, 4, 4, 4, 4, 0, 44, 44, 
	45, 0, 46, 46, 47, 48, 0, 46, 
	46, 48, 0, 49, 48, 48, 48, 48, 
	0, 50, 51, 52, 52, 53, 54, 0, 
	56, 57, 55, 58, 58, 59, 60, 0, 
	61, 61, 62, 0, 62, 62, 50, 52, 
	52, 53, 0, 53, 0, 58, 58, 59, 
	60, 63, 53, 0, 64, 0, 65, 0, 
	58, 58, 59, 60, 65, 0, 55, 66, 
	54, 54, 54, 54, 0, 67, 67, 68, 
	69, 69, 70, 0, 72, 73, 71, 74, 
	74, 75, 76, 0, 77, 77, 51, 54, 
	0, 71, 70, 0, 74, 74, 75, 78, 
	70, 76, 0, 79, 0, 74, 74, 75, 
	79, 76, 0, 47, 47, 80, 80, 81, 
	82, 0, 81, 0, 83, 81, 0, 82, 
	0, 84, 82, 82, 82, 82, 0, 85, 
	86, 87, 87, 88, 89, 0, 91, 92, 
	90, 93, 93, 94, 95, 0, 96, 96, 
	97, 0, 97, 97, 85, 87, 87, 88, 
	0, 88, 0, 93, 93, 94, 95, 98, 
	88, 0, 99, 0, 100, 0, 93, 93, 
	94, 95, 100, 0, 90, 101, 89, 89, 
	89, 89, 0, 102, 102, 103, 104, 104, 
	105, 0, 107, 108, 106, 109, 109, 110, 
	111, 0, 112, 112, 86, 89, 0, 106, 
	105, 0, 109, 109, 110, 113, 105, 111, 
	0, 114, 0, 109, 109, 110, 114, 111, 
	0, 115, 115, 116, 117, 0, 116, 0, 
	118, 118, 119, 116, 0, 120, 120, 121, 
	0, 47, 0, 122, 123, 117, 117, 117, 
	117, 0, 124, 125, 126, 126, 127, 128, 
	0, 130, 131, 129, 132, 132, 133, 134, 
	0, 135, 135, 136, 0, 136, 136, 124, 
	126, 126, 127, 0, 127, 0, 132, 132, 
	133, 134, 137, 127, 0, 138, 0, 139, 
	0, 132, 132, 133, 134, 139, 0, 129, 
	140, 128, 128, 128, 128, 0, 141, 141, 
	142, 143, 143, 144, 0, 146, 147, 145, 
	148, 148, 149, 150, 0, 151, 151, 125, 
	128, 0, 145, 144, 0, 148, 148, 149, 
	152, 144, 150, 0, 153, 0, 148, 148, 
	149, 153, 150, 0, 155, 154, 1, 2, 
	1, 3, 4, 0, 156, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 107, 3, 27, 4, 5, 107, 
	6, 7, 13, 11, 12, 17, 7, 8, 
	16, 9, 13, 10, 9, 10, 14, 107, 
	15, 18, 18, 19, 23, 24, 19, 20, 
	22, 21, 13, 17, 21, 25, 26, 28, 
	107, 58, 79, 29, 28, 29, 30, 53, 
	31, 32, 33, 39, 37, 38, 43, 33, 
	34, 42, 35, 39, 36, 35, 36, 40, 
	107, 41, 44, 44, 45, 49, 50, 45, 
	46, 48, 47, 39, 43, 47, 51, 52, 
	54, 55, 57, 56, 107, 59, 65, 63, 
	64, 69, 59, 60, 68, 61, 65, 62, 
	61, 62, 66, 107, 67, 70, 70, 71, 
	75, 76, 71, 72, 74, 73, 65, 69, 
	73, 77, 78, 80, 81, 84, 82, 83, 
	82, 83, 107, 85, 86, 92, 90, 91, 
	96, 86, 87, 95, 88, 92, 89, 88, 
	89, 93, 107, 94, 97, 97, 98, 102, 
	103, 98, 99, 101, 100, 92, 96, 100, 
	104, 105, 106, 108, 0
};

static const char _command_trans_actions_wi[] = {
	15, 0, 13, 1, 1, 7, 1, 48, 
	3, 0, 0, 1, 1, 1, 1, 0, 
	1, 22, 22, 22, 0, 0, 1, 34, 
	1, 5, 0, 0, 1, 1, 1, 0, 
	1, 25, 25, 56, 0, 1, 1, 19, 
	52, 3, 19, 19, 0, 0, 0, 0, 
	1, 7, 0, 0, 1, 1, 1, 1, 
	0, 1, 22, 22, 22, 0, 0, 1, 
	28, 1, 5, 0, 0, 1, 1, 1, 
	0, 1, 25, 25, 56, 0, 1, 1, 
	1, 1, 1, 11, 40, 0, 0, 1, 
	1, 1, 1, 0, 1, 22, 22, 22, 
	0, 0, 1, 37, 1, 5, 0, 0, 
	1, 1, 1, 0, 1, 25, 25, 56, 
	0, 1, 1, 1, 1, 1, 9, 9, 
	0, 0, 44, 3, 0, 0, 1, 1, 
	1, 1, 0, 1, 22, 22, 22, 0, 
	0, 1, 31, 1, 5, 0, 0, 1, 
	1, 1, 0, 1, 25, 25, 56, 0, 
	1, 1, 0, 17, 0
};

static const int command_start = 1;
static const int command_first_final = 107;

static const int command_en_eat_line = 106;
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
  
  
#line 284 "classes/command.cpp"
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
  
  
#line 319 "classes/command.cpp"
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
      {cs = 106; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 18:
#line 127 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 502 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 169 "classes/command.rl"

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
