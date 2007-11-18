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
	39, 40, 41, 41, 43, 47, 49, 52, 
	60, 67, 69, 74, 79, 79, 81, 89, 
	91, 98, 111, 114, 119, 123, 131, 139, 
	141, 142, 143, 143, 145, 149, 151, 154, 
	162, 169, 171, 176, 181, 181, 183, 191, 
	193, 200, 208, 210, 213, 215, 223, 231, 
	233, 234, 235, 235, 237, 241, 243, 246, 
	254, 261, 263, 268, 273, 273, 275, 283, 
	285, 292, 298, 300, 305, 308, 309, 318, 
	326, 328, 329, 330, 330, 332, 336, 338, 
	341, 349, 356, 358, 363, 368, 368, 370, 
	378, 380, 387, 388, 395
};

static const char _command_trans_keys[] = {
	9, 10, 32, 65, 90, 97, 122, 9, 
	10, 32, 46, 95, 48, 57, 65, 90, 
	97, 122, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 41, 
	10, 48, 57, 41, 46, 48, 57, 48, 
	57, 41, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 34, 43, 
	45, 48, 57, 34, 92, 9, 32, 41, 
	97, 122, 9, 32, 41, 97, 122, 48, 
	57, 9, 32, 41, 46, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 48, 57, 
	97, 122, 9, 10, 32, 40, 46, 61, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 61, 9, 32, 62, 65, 90, 9, 
	32, 65, 90, 40, 95, 48, 57, 65, 
	90, 97, 122, 34, 41, 43, 45, 48, 
	57, 97, 122, 34, 92, 41, 10, 48, 
	57, 41, 46, 48, 57, 48, 57, 41, 
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
	92, 41, 10, 48, 57, 41, 46, 48, 
	57, 48, 57, 41, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 48, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 48, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 43, 45, 48, 57, 
	97, 122, 48, 57, 9, 32, 61, 48, 
	57, 9, 32, 61, 62, 10, 40, 95, 
	48, 57, 65, 90, 97, 122, 34, 41, 
	43, 45, 48, 57, 97, 122, 34, 92, 
	41, 10, 48, 57, 41, 46, 48, 57, 
	48, 57, 41, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 97, 122, 
	48, 57, 9, 32, 41, 46, 48, 57, 
	97, 122, 48, 57, 9, 32, 41, 48, 
	57, 97, 122, 10, 9, 10, 32, 65, 
	90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 2, 0, 3, 4, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 7, 3, 3, 2, 2, 4, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 4, 0, 1, 0, 2, 4, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 2, 0, 3, 3, 1, 3, 4, 
	2, 1, 1, 0, 0, 2, 0, 1, 
	2, 5, 2, 3, 3, 0, 0, 4, 
	0, 3, 1, 3, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 3, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 3, 0, 1, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 2, 1, 1, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 2, 1, 1, 0, 0, 3, 2, 
	0, 0, 0, 0, 1, 1, 1, 1, 
	3, 1, 0, 1, 1, 0, 1, 2, 
	1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 6, 10, 16, 18, 25, 32, 
	35, 37, 39, 40, 42, 46, 48, 51, 
	57, 64, 67, 72, 77, 78, 80, 87, 
	89, 95, 106, 110, 115, 119, 125, 132, 
	135, 137, 139, 140, 142, 146, 148, 151, 
	157, 164, 167, 172, 177, 178, 180, 187, 
	189, 195, 202, 204, 207, 209, 215, 222, 
	225, 227, 229, 230, 232, 236, 238, 241, 
	247, 254, 257, 262, 267, 268, 270, 277, 
	279, 285, 290, 292, 297, 301, 303, 310, 
	317, 320, 322, 324, 325, 327, 331, 333, 
	336, 342, 349, 352, 357, 362, 363, 365, 
	372, 374, 380, 382, 388
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 0, 1, 2, 
	1, 0, 5, 3, 3, 3, 3, 0, 
	6, 0, 7, 8, 6, 6, 6, 6, 
	0, 9, 10, 11, 11, 12, 13, 0, 
	15, 16, 14, 17, 0, 18, 0, 14, 
	12, 0, 17, 19, 12, 0, 20, 0, 
	17, 20, 0, 21, 13, 13, 13, 13, 
	0, 22, 22, 23, 24, 24, 25, 0, 
	27, 28, 26, 29, 29, 30, 31, 0, 
	32, 32, 10, 13, 0, 26, 25, 0, 
	29, 29, 30, 33, 25, 31, 0, 34, 
	0, 29, 29, 30, 34, 31, 0, 35, 
	36, 35, 37, 38, 39, 4, 4, 4, 
	4, 0, 40, 40, 41, 0, 42, 42, 
	43, 44, 0, 42, 42, 44, 0, 45, 
	44, 44, 44, 44, 0, 46, 47, 48, 
	48, 49, 50, 0, 52, 53, 51, 54, 
	0, 55, 0, 51, 49, 0, 54, 56, 
	49, 0, 57, 0, 54, 57, 0, 58, 
	50, 50, 50, 50, 0, 59, 59, 60, 
	61, 61, 62, 0, 64, 65, 63, 66, 
	66, 67, 68, 0, 69, 69, 47, 50, 
	0, 63, 62, 0, 66, 66, 67, 70, 
	62, 68, 0, 71, 0, 66, 66, 67, 
	71, 68, 0, 43, 43, 72, 72, 73, 
	74, 0, 73, 0, 75, 73, 0, 74, 
	0, 76, 74, 74, 74, 74, 0, 77, 
	78, 79, 79, 80, 81, 0, 83, 84, 
	82, 85, 0, 86, 0, 82, 80, 0, 
	85, 87, 80, 0, 88, 0, 85, 88, 
	0, 89, 81, 81, 81, 81, 0, 90, 
	90, 91, 92, 92, 93, 0, 95, 96, 
	94, 97, 97, 98, 99, 0, 100, 100, 
	78, 81, 0, 94, 93, 0, 97, 97, 
	98, 101, 93, 99, 0, 102, 0, 97, 
	97, 98, 102, 99, 0, 103, 103, 104, 
	105, 0, 104, 0, 106, 106, 107, 104, 
	0, 108, 108, 109, 0, 43, 0, 110, 
	111, 105, 105, 105, 105, 0, 112, 113, 
	114, 114, 115, 116, 0, 118, 119, 117, 
	120, 0, 121, 0, 117, 115, 0, 120, 
	122, 115, 0, 123, 0, 120, 123, 0, 
	124, 116, 116, 116, 116, 0, 125, 125, 
	126, 127, 127, 128, 0, 130, 131, 129, 
	132, 132, 133, 134, 0, 135, 135, 113, 
	116, 0, 129, 128, 0, 132, 132, 133, 
	136, 128, 134, 0, 137, 0, 132, 132, 
	133, 137, 134, 0, 139, 138, 1, 2, 
	1, 3, 4, 0, 140, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 99, 3, 25, 4, 5, 99, 
	6, 7, 9, 11, 12, 15, 7, 8, 
	10, 9, 99, 13, 14, 16, 16, 17, 
	21, 22, 17, 18, 20, 19, 9, 15, 
	19, 23, 24, 26, 99, 54, 73, 27, 
	26, 27, 28, 49, 29, 30, 31, 33, 
	35, 36, 39, 31, 32, 34, 33, 99, 
	37, 38, 40, 40, 41, 45, 46, 41, 
	42, 44, 43, 33, 39, 43, 47, 48, 
	50, 51, 53, 52, 99, 55, 57, 59, 
	60, 63, 55, 56, 58, 57, 99, 61, 
	62, 64, 64, 65, 69, 70, 65, 66, 
	68, 67, 57, 63, 67, 71, 72, 74, 
	75, 78, 76, 77, 76, 77, 99, 79, 
	80, 82, 84, 85, 88, 80, 81, 83, 
	82, 99, 86, 87, 89, 89, 90, 94, 
	95, 90, 91, 93, 92, 82, 88, 92, 
	96, 97, 98, 100, 0
};

static const char _command_trans_actions_wi[] = {
	15, 0, 13, 1, 1, 7, 1, 48, 
	3, 0, 0, 1, 1, 1, 1, 0, 
	1, 22, 34, 1, 1, 5, 0, 0, 
	1, 1, 1, 0, 1, 25, 25, 56, 
	0, 1, 1, 19, 52, 3, 19, 19, 
	0, 0, 0, 0, 1, 7, 0, 0, 
	1, 1, 1, 1, 0, 1, 22, 28, 
	1, 1, 5, 0, 0, 1, 1, 1, 
	0, 1, 25, 25, 56, 0, 1, 1, 
	1, 1, 1, 11, 40, 0, 0, 1, 
	1, 1, 1, 0, 1, 22, 37, 1, 
	1, 5, 0, 0, 1, 1, 1, 0, 
	1, 25, 25, 56, 0, 1, 1, 1, 
	1, 1, 9, 9, 0, 0, 44, 3, 
	0, 0, 1, 1, 1, 1, 0, 1, 
	22, 31, 1, 1, 5, 0, 0, 1, 
	1, 1, 0, 1, 25, 25, 56, 0, 
	1, 1, 0, 17, 0
};

static const int command_start = 1;
static const int command_first_final = 99;

static const int command_en_eat_line = 98;
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
  
  
#line 257 "classes/command.cpp"
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
  
  
#line 292 "classes/command.cpp"
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
	{ set_class_from_token();}
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
	{ set_single_param_from_token(); }
	break;
	case 10:
#line 94 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 96 "classes/command.rl"
	{
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 101 "classes/command.rl"
	{ create_instance(); }
	break;
	case 13:
#line 103 "classes/command.rl"
	{ execute_method(); }
	break;
	case 14:
#line 105 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 15:
#line 107 "classes/command.rl"
	{ execute_command(); }
	break;
	case 16:
#line 111 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
	case 17:
#line 117 "classes/command.rl"
	{
      p--; // move back one char
      *mOutput << "Syntax error !" << std::endl;
      clear();
      prompt();
      {cs = 98; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 18:
#line 125 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 473 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 167 "classes/command.rl"

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
