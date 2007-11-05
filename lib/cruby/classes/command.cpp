#line 1 "classes/command.rl"
#include "command.h"
#include "rubyk.h"
#include "class.h"

#undef DEBUG_PARSER


#line 10 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	15, 1, 16, 1, 17, 2, 5, 8, 
	2, 5, 9, 2, 11, 15, 2, 12, 
	15, 2, 13, 15, 2, 14, 15, 3, 
	2, 12, 15, 3, 2, 13, 15, 3, 
	2, 14, 15, 3, 5, 9, 0, 4, 
	1, 7, 10, 15
};

static const short _command_key_offsets[] = {
	0, 0, 7, 10, 18, 20, 29, 37, 
	39, 40, 41, 41, 43, 47, 49, 52, 
	60, 67, 69, 74, 79, 79, 81, 89, 
	91, 98, 111, 114, 118, 126, 134, 136, 
	137, 138, 138, 140, 144, 146, 149, 157, 
	164, 166, 171, 176, 176, 178, 186, 188, 
	195, 203, 205, 206, 207, 207, 209, 213, 
	215, 218, 226, 233, 235, 240, 245, 245, 
	247, 255, 257, 264, 270, 272, 277, 280, 
	281, 287, 289, 292, 294, 302, 311, 319, 
	321, 322, 323, 323, 325, 329, 331, 334, 
	342, 349, 351, 356, 361, 361, 363, 371, 
	373, 380, 381, 388
};

static const char _command_trans_keys[] = {
	9, 10, 32, 65, 90, 97, 122, 9, 
	10, 32, 46, 95, 48, 57, 65, 90, 
	97, 122, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 49, 57, 97, 122, 34, 92, 41, 
	10, 49, 57, 41, 46, 48, 57, 48, 
	57, 41, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 34, 43, 
	45, 49, 57, 34, 92, 9, 32, 41, 
	97, 122, 9, 32, 41, 97, 122, 49, 
	57, 9, 32, 41, 46, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 48, 57, 
	97, 122, 9, 10, 32, 40, 46, 61, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 61, 9, 32, 65, 90, 40, 95, 
	48, 57, 65, 90, 97, 122, 34, 41, 
	43, 45, 49, 57, 97, 122, 34, 92, 
	41, 10, 49, 57, 41, 46, 48, 57, 
	48, 57, 41, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 49, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 97, 122, 
	49, 57, 9, 32, 41, 46, 48, 57, 
	97, 122, 48, 57, 9, 32, 41, 48, 
	57, 97, 122, 34, 41, 43, 45, 49, 
	57, 97, 122, 34, 92, 41, 10, 49, 
	57, 41, 46, 48, 57, 48, 57, 41, 
	48, 57, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 34, 43, 45, 49, 
	57, 34, 92, 9, 32, 41, 97, 122, 
	9, 32, 41, 97, 122, 49, 57, 9, 
	32, 41, 46, 48, 57, 97, 122, 48, 
	57, 9, 32, 41, 48, 57, 97, 122, 
	43, 45, 49, 57, 97, 122, 49, 57, 
	9, 32, 61, 48, 57, 9, 32, 61, 
	62, 9, 32, 43, 45, 49, 57, 49, 
	57, 46, 48, 57, 97, 122, 10, 95, 
	48, 57, 65, 90, 97, 122, 10, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	41, 43, 45, 49, 57, 97, 122, 34, 
	92, 41, 10, 49, 57, 41, 46, 48, 
	57, 48, 57, 41, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 43, 45, 49, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 49, 57, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 10, 9, 10, 32, 
	65, 90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 2, 0, 3, 4, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 7, 3, 2, 2, 4, 2, 1, 
	1, 0, 0, 2, 0, 1, 2, 5, 
	2, 3, 3, 0, 0, 4, 0, 3, 
	4, 2, 1, 1, 0, 0, 2, 0, 
	1, 2, 5, 2, 3, 3, 0, 0, 
	4, 0, 3, 2, 0, 3, 3, 1, 
	4, 0, 1, 0, 2, 3, 4, 2, 
	1, 1, 0, 0, 2, 0, 1, 2, 
	5, 2, 3, 3, 0, 0, 4, 0, 
	3, 1, 3, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 3, 1, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 3, 0, 1, 3, 2, 0, 0, 
	0, 0, 1, 1, 1, 1, 3, 1, 
	0, 1, 1, 0, 1, 2, 1, 2, 
	2, 0, 0, 0, 0, 1, 1, 1, 
	1, 3, 1, 0, 1, 1, 0, 1, 
	2, 1, 2, 2, 1, 1, 0, 0, 
	1, 1, 1, 1, 3, 3, 2, 0, 
	0, 0, 0, 1, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 1, 2, 1, 
	2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 6, 10, 16, 18, 25, 32, 
	35, 37, 39, 40, 42, 46, 48, 51, 
	57, 64, 67, 72, 77, 78, 80, 87, 
	89, 95, 106, 110, 114, 120, 127, 130, 
	132, 134, 135, 137, 141, 143, 146, 152, 
	159, 162, 167, 172, 173, 175, 182, 184, 
	190, 197, 200, 202, 204, 205, 207, 211, 
	213, 216, 222, 229, 232, 237, 242, 243, 
	245, 252, 254, 260, 265, 267, 272, 276, 
	278, 284, 286, 289, 291, 297, 304, 311, 
	314, 316, 318, 319, 321, 325, 327, 330, 
	336, 343, 346, 351, 356, 357, 359, 366, 
	368, 374, 376, 382
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
	4, 0, 40, 40, 41, 0, 41, 41, 
	42, 0, 43, 42, 42, 42, 42, 0, 
	44, 45, 46, 46, 47, 48, 0, 50, 
	51, 49, 52, 0, 53, 0, 49, 47, 
	0, 52, 54, 47, 0, 55, 0, 52, 
	55, 0, 56, 48, 48, 48, 48, 0, 
	57, 57, 58, 59, 59, 60, 0, 62, 
	63, 61, 64, 64, 65, 66, 0, 67, 
	67, 45, 48, 0, 61, 60, 0, 64, 
	64, 65, 68, 60, 66, 0, 69, 0, 
	64, 64, 65, 69, 66, 0, 70, 71, 
	72, 72, 73, 74, 0, 76, 77, 75, 
	78, 0, 79, 0, 75, 73, 0, 78, 
	80, 73, 0, 81, 0, 78, 81, 0, 
	82, 74, 74, 74, 74, 0, 83, 83, 
	84, 85, 85, 86, 0, 88, 89, 87, 
	90, 90, 91, 92, 0, 93, 93, 71, 
	74, 0, 87, 86, 0, 90, 90, 91, 
	94, 86, 92, 0, 95, 0, 90, 90, 
	91, 95, 92, 0, 96, 96, 97, 98, 
	0, 97, 0, 99, 99, 100, 97, 0, 
	101, 101, 102, 0, 103, 0, 103, 103, 
	104, 104, 105, 0, 105, 0, 106, 105, 
	0, 107, 0, 108, 107, 107, 107, 107, 
	0, 109, 110, 98, 98, 98, 98, 0, 
	111, 112, 113, 113, 114, 115, 0, 117, 
	118, 116, 119, 0, 120, 0, 116, 114, 
	0, 119, 121, 114, 0, 122, 0, 119, 
	122, 0, 123, 115, 115, 115, 115, 0, 
	124, 124, 125, 126, 126, 127, 0, 129, 
	130, 128, 131, 131, 132, 133, 0, 134, 
	134, 112, 115, 0, 128, 127, 0, 131, 
	131, 132, 135, 127, 133, 0, 136, 0, 
	131, 131, 132, 136, 133, 0, 138, 137, 
	1, 2, 1, 3, 4, 0, 139, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 98, 3, 25, 4, 5, 98, 
	6, 7, 9, 11, 12, 15, 7, 8, 
	10, 9, 98, 13, 14, 16, 16, 17, 
	21, 22, 17, 18, 20, 19, 9, 15, 
	19, 23, 24, 26, 98, 48, 67, 27, 
	26, 27, 28, 29, 30, 32, 34, 35, 
	38, 30, 31, 33, 32, 98, 36, 37, 
	39, 39, 40, 44, 45, 40, 41, 43, 
	42, 32, 38, 42, 46, 47, 49, 51, 
	53, 54, 57, 49, 50, 52, 51, 98, 
	55, 56, 58, 58, 59, 63, 64, 59, 
	60, 62, 61, 51, 57, 61, 65, 66, 
	68, 69, 77, 70, 71, 70, 71, 72, 
	73, 74, 75, 76, 98, 98, 78, 79, 
	81, 83, 84, 87, 79, 80, 82, 81, 
	98, 85, 86, 88, 88, 89, 93, 94, 
	89, 90, 92, 91, 81, 87, 91, 95, 
	96, 97, 99, 0
};

static const char _command_trans_actions_wi[] = {
	17, 0, 15, 1, 1, 9, 1, 43, 
	5, 0, 0, 1, 1, 1, 1, 0, 
	1, 21, 33, 1, 1, 7, 0, 0, 
	1, 1, 1, 0, 1, 24, 24, 51, 
	0, 1, 1, 3, 47, 5, 3, 3, 
	0, 0, 1, 9, 0, 0, 1, 1, 
	1, 1, 0, 1, 21, 27, 1, 1, 
	7, 0, 0, 1, 1, 1, 0, 1, 
	24, 24, 51, 0, 1, 1, 0, 0, 
	1, 1, 1, 1, 0, 1, 21, 36, 
	1, 1, 7, 0, 0, 1, 1, 1, 
	0, 1, 24, 24, 51, 0, 1, 1, 
	1, 1, 1, 13, 13, 0, 0, 0, 
	1, 1, 11, 1, 55, 39, 5, 0, 
	0, 1, 1, 1, 1, 0, 1, 21, 
	30, 1, 1, 7, 0, 0, 1, 1, 
	1, 0, 1, 24, 24, 51, 0, 1, 
	1, 0, 19, 0
};

static const int command_start = 1;
static const int command_first_final = 98;

static const int command_en_eat_line = 97;
static const int command_en_main = 1;

#line 10 "classes/command.rl"


Command::Command(Rubyk * pServer)
{
  int cs;
  mAction = NO_ACTION;
  mServer = pServer;
  mThread = 0;
  mQuit   = false;
  
#line 248 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 20 "classes/command.rl"
  mCurrentState = cs;
  mTokenIndex = 0;
  mInput  = &std::cin;
  mOutput = &std::cout;
}

Command::~Command()
{
  if (mThread) close();
}

void Command::listen (std::istream& pInput, std::ostream& pOutput)
{
  int ret;
  if (mThread) {
    close();
    mQuit = false;
  }
  mOutput = &pOutput;
  mInput  = &pInput;
  ret = pthread_create( &mThread, NULL, &Command::call_do_listen, this);
  // FIXME: check for error from 'ret'
}

int Command::do_listen()
{
  char iss[1024];
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
  

  
#line 300 "classes/command.cpp"
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
#line 67 "classes/command.rl"
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
#line 80 "classes/command.rl"
	{ set_from_token(mVariable);}
	break;
	case 2:
#line 82 "classes/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 84 "classes/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 86 "classes/command.rl"
	{ set_class_from_token();}
	break;
	case 5:
#line 88 "classes/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 90 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 7:
#line 96 "classes/command.rl"
	{
      set_from_token(mTo);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 101 "classes/command.rl"
	{ set_single_param_from_token(); }
	break;
	case 9:
#line 103 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 10:
#line 105 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
	break;
	case 11:
#line 111 "classes/command.rl"
	{ create_instance(); }
	break;
	case 12:
#line 113 "classes/command.rl"
	{ execute_method(); }
	break;
	case 13:
#line 115 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 14:
#line 117 "classes/command.rl"
	{ execute_command(); }
	break;
	case 15:
#line 121 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
	case 16:
#line 127 "classes/command.rl"
	{
      p--; // move back one char
      *mOutput << "Syntax error !" << std::endl;
      clear();
      prompt();
      {cs = 97; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 17:
#line 135 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 479 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 176 "classes/command.rl"

//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close() {
  mQuit = true;
  // FIXME: how to force out of input (>>) ?
  if (mThread)
    pthread_join( mThread, NULL); // wait for listen to finish
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
  Node * node = mServer->create_instance(mVariable, mClass, mParameters, mOutput);
#ifdef DEBUG_PARSER
  std::cout << "NEW("<< mVariable << ", " << mClass << ", " << mParameters << ")";
#endif
  if (node) {
    if (!mSilent)
      *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
}


void Command::create_link()
{  
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  if (!mSilent)
    *mOutput << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node;
  if (mServer->get_instance(&node, mVariable)) {
    node->execute_method(mMethod, mParameters);
    if (!mSilent && mMethod == "bang") *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
}

// FIXME: execute_class_method should run in server space with concurrency locks.
void Command::execute_class_method()
{
  Class * klass;
  if (Class::get(&klass, mClass)) {
    klass->execute_method(mMethod, mParameters, mOutput);
  } else {
    *mOutput << "Unknown class '" << mClass << "'" << std::endl;
  }
}

// FIXME: execute_command should run in server space with concurrency locks.
void Command::execute_command()
{
  Node * node;
  if (mServer->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
  } else if (mMethod == "quit") {
    mServer->quit(); // not all commands should quit the server...
    mQuit = true;
    *mOutput << "Bye..." << std::endl;
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
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
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mSingleParam = "";
  mParameters.clear();
}
