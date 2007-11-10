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
	0, 0, 7, 10, 18, 20, 29, 38, 
	40, 41, 42, 42, 44, 48, 50, 53, 
	61, 69, 71, 76, 81, 81, 83, 91, 
	93, 100, 113, 116, 121, 125, 133, 142, 
	144, 145, 146, 146, 148, 152, 154, 157, 
	165, 173, 175, 180, 185, 185, 187, 195, 
	197, 204, 213, 215, 218, 220, 228, 229, 
	238, 240, 241, 242, 242, 244, 248, 250, 
	253, 261, 269, 271, 276, 281, 281, 283, 
	291, 293, 300, 307, 309, 314, 317, 318, 
	321, 330, 339, 341, 342, 343, 343, 345, 
	349, 351, 354, 362, 370, 372, 377, 382, 
	382, 384, 392, 394, 401, 402, 409
};

static const char _command_trans_keys[] = {
	9, 10, 32, 65, 90, 97, 122, 9, 
	10, 32, 46, 95, 48, 57, 65, 90, 
	97, 122, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 49, 57, 97, 122, 34, 92, 
	41, 10, 49, 57, 41, 46, 48, 57, 
	48, 57, 41, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 49, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 49, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 9, 10, 32, 40, 
	46, 61, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 61, 9, 32, 62, 65, 
	90, 9, 32, 65, 90, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 49, 57, 97, 122, 34, 92, 
	41, 10, 49, 57, 41, 46, 48, 57, 
	48, 57, 41, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 49, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 49, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 9, 32, 43, 45, 
	48, 49, 57, 97, 122, 49, 57, 46, 
	48, 57, 97, 122, 10, 95, 48, 57, 
	65, 90, 97, 122, 46, 34, 41, 43, 
	45, 48, 49, 57, 97, 122, 34, 92, 
	41, 10, 49, 57, 41, 46, 48, 57, 
	48, 57, 41, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 49, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 49, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 43, 45, 48, 49, 
	57, 97, 122, 49, 57, 9, 32, 61, 
	48, 57, 9, 32, 61, 62, 9, 32, 
	61, 10, 40, 95, 48, 57, 65, 90, 
	97, 122, 34, 41, 43, 45, 48, 49, 
	57, 97, 122, 34, 92, 41, 10, 49, 
	57, 41, 46, 48, 57, 48, 57, 41, 
	48, 57, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 34, 43, 45, 48, 
	49, 57, 34, 92, 9, 32, 41, 97, 
	122, 9, 32, 41, 97, 122, 49, 57, 
	9, 32, 41, 46, 48, 57, 97, 122, 
	48, 57, 9, 32, 41, 48, 57, 97, 
	122, 10, 9, 10, 32, 65, 90, 97, 
	122, 0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 2, 0, 3, 5, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	6, 2, 3, 3, 0, 0, 4, 0, 
	3, 7, 3, 3, 2, 2, 5, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	6, 2, 3, 3, 0, 0, 4, 0, 
	3, 5, 0, 1, 0, 2, 1, 5, 
	2, 1, 1, 0, 0, 2, 0, 1, 
	2, 6, 2, 3, 3, 0, 0, 4, 
	0, 3, 3, 0, 3, 3, 1, 3, 
	3, 5, 2, 1, 1, 0, 0, 2, 
	0, 1, 2, 6, 2, 3, 3, 0, 
	0, 4, 0, 3, 1, 3, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 3, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 3, 0, 1, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 2, 1, 1, 1, 3, 0, 2, 
	0, 0, 0, 0, 1, 1, 1, 1, 
	3, 1, 0, 1, 1, 0, 1, 2, 
	1, 2, 2, 1, 1, 0, 0, 0, 
	3, 2, 0, 0, 0, 0, 1, 1, 
	1, 1, 3, 1, 0, 1, 1, 0, 
	1, 2, 1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 6, 10, 16, 18, 25, 33, 
	36, 38, 40, 41, 43, 47, 49, 52, 
	58, 66, 69, 74, 79, 80, 82, 89, 
	91, 97, 108, 112, 117, 121, 127, 135, 
	138, 140, 142, 143, 145, 149, 151, 154, 
	160, 168, 171, 176, 181, 182, 184, 191, 
	193, 199, 207, 209, 212, 214, 220, 222, 
	230, 233, 235, 237, 238, 240, 244, 246, 
	249, 255, 263, 266, 271, 276, 277, 279, 
	286, 288, 294, 300, 302, 307, 311, 313, 
	317, 324, 332, 335, 337, 339, 340, 342, 
	346, 348, 351, 357, 365, 368, 373, 378, 
	379, 381, 388, 390, 396, 398, 404
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 0, 1, 2, 
	1, 0, 5, 3, 3, 3, 3, 0, 
	6, 0, 7, 8, 6, 6, 6, 6, 
	0, 9, 10, 11, 11, 12, 13, 14, 
	0, 16, 17, 15, 18, 0, 19, 0, 
	15, 13, 0, 18, 20, 13, 0, 21, 
	0, 18, 21, 0, 22, 14, 14, 14, 
	14, 0, 23, 23, 24, 25, 25, 26, 
	27, 0, 29, 30, 28, 31, 31, 32, 
	33, 0, 34, 34, 10, 14, 0, 28, 
	27, 0, 31, 31, 32, 35, 27, 33, 
	0, 36, 0, 31, 31, 32, 36, 33, 
	0, 37, 38, 37, 39, 40, 41, 4, 
	4, 4, 4, 0, 42, 42, 43, 0, 
	44, 44, 45, 46, 0, 44, 44, 46, 
	0, 47, 46, 46, 46, 46, 0, 48, 
	49, 50, 50, 51, 52, 53, 0, 55, 
	56, 54, 57, 0, 58, 0, 54, 52, 
	0, 57, 59, 52, 0, 60, 0, 57, 
	60, 0, 61, 53, 53, 53, 53, 0, 
	62, 62, 63, 64, 64, 65, 66, 0, 
	68, 69, 67, 70, 70, 71, 72, 0, 
	73, 73, 49, 53, 0, 67, 66, 0, 
	70, 70, 71, 74, 66, 72, 0, 75, 
	0, 70, 70, 71, 75, 72, 0, 45, 
	45, 76, 76, 77, 78, 79, 0, 78, 
	0, 80, 78, 0, 79, 0, 81, 79, 
	79, 79, 79, 0, 80, 0, 82, 83, 
	84, 84, 85, 86, 87, 0, 89, 90, 
	88, 91, 0, 92, 0, 88, 86, 0, 
	91, 93, 86, 0, 94, 0, 91, 94, 
	0, 95, 87, 87, 87, 87, 0, 96, 
	96, 97, 98, 98, 99, 100, 0, 102, 
	103, 101, 104, 104, 105, 106, 0, 107, 
	107, 83, 87, 0, 101, 100, 0, 104, 
	104, 105, 108, 100, 106, 0, 109, 0, 
	104, 104, 105, 109, 106, 0, 110, 110, 
	111, 112, 113, 0, 112, 0, 114, 114, 
	115, 112, 0, 116, 116, 117, 0, 45, 
	0, 114, 114, 115, 0, 118, 119, 113, 
	113, 113, 113, 0, 120, 121, 122, 122, 
	123, 124, 125, 0, 127, 128, 126, 129, 
	0, 130, 0, 126, 124, 0, 129, 131, 
	124, 0, 132, 0, 129, 132, 0, 133, 
	125, 125, 125, 125, 0, 134, 134, 135, 
	136, 136, 137, 138, 0, 140, 141, 139, 
	142, 142, 143, 144, 0, 145, 145, 121, 
	125, 0, 139, 138, 0, 142, 142, 143, 
	146, 138, 144, 0, 147, 0, 142, 142, 
	143, 147, 144, 0, 149, 148, 1, 2, 
	1, 3, 4, 0, 150, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 101, 3, 25, 4, 5, 101, 
	6, 7, 9, 11, 8, 12, 15, 7, 
	8, 10, 9, 101, 13, 14, 16, 16, 
	17, 21, 18, 22, 17, 18, 20, 19, 
	9, 15, 19, 23, 24, 26, 101, 55, 
	74, 27, 26, 27, 28, 49, 29, 30, 
	31, 33, 35, 32, 36, 39, 31, 32, 
	34, 33, 101, 37, 38, 40, 40, 41, 
	45, 42, 46, 41, 42, 44, 43, 33, 
	39, 43, 47, 48, 50, 54, 51, 53, 
	52, 101, 56, 58, 60, 57, 61, 64, 
	56, 57, 59, 58, 101, 62, 63, 65, 
	65, 66, 70, 67, 71, 66, 67, 69, 
	68, 58, 64, 68, 72, 73, 75, 79, 
	76, 80, 77, 78, 77, 78, 101, 81, 
	82, 84, 86, 83, 87, 90, 82, 83, 
	85, 84, 101, 88, 89, 91, 91, 92, 
	96, 93, 97, 92, 93, 95, 94, 84, 
	90, 94, 98, 99, 100, 102, 0
};

static const char _command_trans_actions_wi[] = {
	15, 0, 13, 1, 1, 7, 1, 48, 
	3, 0, 0, 1, 1, 1, 1, 1, 
	0, 1, 22, 34, 1, 1, 5, 0, 
	0, 1, 1, 1, 1, 0, 1, 25, 
	25, 56, 0, 1, 1, 19, 52, 3, 
	19, 19, 0, 0, 0, 0, 1, 7, 
	0, 0, 1, 1, 1, 1, 1, 0, 
	1, 22, 28, 1, 1, 5, 0, 0, 
	1, 1, 1, 1, 0, 1, 25, 25, 
	56, 0, 1, 1, 1, 1, 1, 1, 
	11, 40, 0, 0, 1, 1, 1, 1, 
	1, 0, 1, 22, 37, 1, 1, 5, 
	0, 0, 1, 1, 1, 1, 0, 1, 
	25, 25, 56, 0, 1, 1, 1, 1, 
	1, 1, 9, 9, 0, 0, 44, 3, 
	0, 0, 1, 1, 1, 1, 1, 0, 
	1, 22, 31, 1, 1, 5, 0, 0, 
	1, 1, 1, 1, 0, 1, 25, 25, 
	56, 0, 1, 1, 0, 17, 0
};

static const int command_start = 1;
static const int command_first_final = 101;

static const int command_en_eat_line = 100;
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
  
  clear();
  
  
#line 262 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 26 "classes/command.rl"
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
  
  
#line 297 "classes/command.cpp"
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
#line 56 "classes/command.rl"
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
#line 69 "classes/command.rl"
	{ set_from_token(mVariable);}
	break;
	case 2:
#line 71 "classes/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 73 "classes/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 75 "classes/command.rl"
	{ set_class_from_token();}
	break;
	case 5:
#line 77 "classes/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 79 "classes/command.rl"
	{ mFrom = mVariable; }
	break;
	case 7:
#line 81 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 86 "classes/command.rl"
	{
      set_from_token(mTo);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 9:
#line 91 "classes/command.rl"
	{ set_single_param_from_token(); }
	break;
	case 10:
#line 93 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 95 "classes/command.rl"
	{
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 100 "classes/command.rl"
	{ create_instance(); }
	break;
	case 13:
#line 102 "classes/command.rl"
	{ execute_method(); }
	break;
	case 14:
#line 104 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 15:
#line 106 "classes/command.rl"
	{ execute_command(); }
	break;
	case 16:
#line 110 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
	case 17:
#line 116 "classes/command.rl"
	{
      p--; // move back one char
      *mOutput << "Syntax error !" << std::endl;
      clear();
      prompt();
      {cs = 100; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 18:
#line 124 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 478 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 166 "classes/command.rl"

//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close()
{
  mQuit = true;
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


void Command::set_class_from_token  () 
{
  set_from_token(mClass);
  // FIXME: this might not be needed if class is set before param
  if (mSingleParam != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
    mSingleParam = "";
  }
}

void Command::set_single_param_from_token () 
{
  mSingleParam = mValue;
  std::string key;
  if (mClass != "") {
    key = mClass;
  } else if (mMethod != "") {
    key = mMethod;
  } else {
    key = "value";
  }
  std::transform(key.begin(), key.end(), key.begin(), tolower);
  mParameters.set(key, mSingleParam);
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
  mSingleParam = "";
  mParameters.clear();
  mFromPort = 1;
  mToPort   = 1;
}
