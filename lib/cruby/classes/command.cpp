#line 1 "classes/command.rl"
#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"

#undef DEBUG_PARSER


#line 12 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	14, 1, 15, 1, 16, 2, 5, 9, 
	2, 5, 10, 2, 8, 14, 2, 12, 
	14, 2, 13, 14, 3, 2, 12, 14, 
	3, 2, 13, 14, 3, 5, 10, 0, 
	4, 1, 7, 11, 14
};

static const short _command_key_offsets[] = {
	0, 0, 5, 8, 21, 24, 28, 36, 
	44, 46, 47, 48, 48, 50, 54, 56, 
	59, 67, 74, 76, 81, 86, 86, 88, 
	96, 98, 105, 113, 115, 116, 117, 117, 
	119, 123, 125, 128, 136, 143, 145, 150, 
	155, 155, 157, 165, 167, 174, 180, 182, 
	187, 190, 191, 197, 199, 202, 204, 212, 
	221, 229, 231, 232, 233, 233, 235, 239, 
	241, 244, 252, 259, 261, 266, 271, 271, 
	273, 281, 283, 290, 291, 296
};

static const char _command_trans_keys[] = {
	9, 10, 32, 97, 122, 9, 10, 32, 
	9, 10, 32, 40, 46, 61, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 61, 
	9, 32, 65, 90, 40, 95, 48, 57, 
	65, 90, 97, 122, 34, 41, 43, 45, 
	49, 57, 97, 122, 34, 92, 41, 10, 
	49, 57, 41, 46, 48, 57, 48, 57, 
	41, 48, 57, 58, 95, 48, 57, 65, 
	90, 97, 122, 9, 32, 34, 43, 45, 
	49, 57, 34, 92, 9, 32, 41, 97, 
	122, 9, 32, 41, 97, 122, 49, 57, 
	9, 32, 41, 46, 48, 57, 97, 122, 
	48, 57, 9, 32, 41, 48, 57, 97, 
	122, 34, 41, 43, 45, 49, 57, 97, 
	122, 34, 92, 41, 10, 49, 57, 41, 
	46, 48, 57, 48, 57, 41, 48, 57, 
	58, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 34, 43, 45, 49, 57, 34, 
	92, 9, 32, 41, 97, 122, 9, 32, 
	41, 97, 122, 49, 57, 9, 32, 41, 
	46, 48, 57, 97, 122, 48, 57, 9, 
	32, 41, 48, 57, 97, 122, 43, 45, 
	49, 57, 97, 122, 49, 57, 9, 32, 
	61, 48, 57, 9, 32, 61, 62, 9, 
	32, 43, 45, 49, 57, 49, 57, 46, 
	48, 57, 97, 122, 10, 95, 48, 57, 
	65, 90, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 43, 
	45, 49, 57, 97, 122, 34, 92, 41, 
	10, 49, 57, 41, 46, 48, 57, 48, 
	57, 41, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 34, 43, 
	45, 49, 57, 34, 92, 9, 32, 41, 
	97, 122, 9, 32, 41, 97, 122, 49, 
	57, 9, 32, 41, 46, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 48, 57, 
	97, 122, 10, 9, 10, 32, 97, 122, 
	0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 7, 3, 2, 2, 4, 
	2, 1, 1, 0, 0, 2, 0, 1, 
	2, 5, 2, 3, 3, 0, 0, 4, 
	0, 3, 4, 2, 1, 1, 0, 0, 
	2, 0, 1, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 2, 0, 3, 
	3, 1, 4, 0, 1, 0, 2, 3, 
	4, 2, 1, 1, 0, 0, 2, 0, 
	1, 2, 5, 2, 3, 3, 0, 0, 
	4, 0, 3, 1, 3, 0
};

static const char _command_range_lengths[] = {
	0, 1, 0, 3, 0, 1, 3, 2, 
	0, 0, 0, 0, 1, 1, 1, 1, 
	3, 1, 0, 1, 1, 0, 1, 2, 
	1, 2, 2, 0, 0, 0, 0, 1, 
	1, 1, 1, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 2, 1, 1, 
	0, 0, 1, 1, 1, 1, 3, 3, 
	2, 0, 0, 0, 0, 1, 1, 1, 
	1, 3, 1, 0, 1, 1, 0, 1, 
	2, 1, 2, 0, 1, 0
};

static const short _command_index_offsets[] = {
	0, 0, 5, 9, 20, 24, 28, 34, 
	41, 44, 46, 48, 49, 51, 55, 57, 
	60, 66, 73, 76, 81, 86, 87, 89, 
	96, 98, 104, 111, 114, 116, 118, 119, 
	121, 125, 127, 130, 136, 143, 146, 151, 
	156, 157, 159, 166, 168, 174, 179, 181, 
	186, 190, 192, 198, 200, 203, 205, 211, 
	218, 225, 228, 230, 232, 233, 235, 239, 
	241, 244, 250, 257, 260, 265, 270, 271, 
	273, 280, 282, 288, 290, 295
};

static const char _command_indicies[] = {
	1, 2, 1, 3, 0, 1, 2, 1, 
	0, 4, 5, 4, 6, 7, 8, 3, 
	3, 3, 3, 0, 9, 9, 10, 0, 
	10, 10, 11, 0, 12, 11, 11, 11, 
	11, 0, 13, 14, 15, 15, 16, 17, 
	0, 19, 20, 18, 21, 0, 22, 0, 
	18, 16, 0, 21, 23, 16, 0, 24, 
	0, 21, 24, 0, 25, 17, 17, 17, 
	17, 0, 26, 26, 27, 28, 28, 29, 
	0, 31, 32, 30, 33, 33, 34, 35, 
	0, 36, 36, 14, 17, 0, 30, 29, 
	0, 33, 33, 34, 37, 29, 35, 0, 
	38, 0, 33, 33, 34, 38, 35, 0, 
	39, 40, 41, 41, 42, 43, 0, 45, 
	46, 44, 47, 0, 48, 0, 44, 42, 
	0, 47, 49, 42, 0, 50, 0, 47, 
	50, 0, 51, 43, 43, 43, 43, 0, 
	52, 52, 53, 54, 54, 55, 0, 57, 
	58, 56, 59, 59, 60, 61, 0, 62, 
	62, 40, 43, 0, 56, 55, 0, 59, 
	59, 60, 63, 55, 61, 0, 64, 0, 
	59, 59, 60, 64, 61, 0, 65, 65, 
	66, 67, 0, 66, 0, 68, 68, 69, 
	66, 0, 70, 70, 71, 0, 72, 0, 
	72, 72, 73, 73, 74, 0, 74, 0, 
	75, 74, 0, 76, 0, 77, 76, 76, 
	76, 76, 0, 78, 79, 67, 67, 67, 
	67, 0, 80, 81, 82, 82, 83, 84, 
	0, 86, 87, 85, 88, 0, 89, 0, 
	85, 83, 0, 88, 90, 83, 0, 91, 
	0, 88, 91, 0, 92, 84, 84, 84, 
	84, 0, 93, 93, 94, 95, 95, 96, 
	0, 98, 99, 97, 100, 100, 101, 102, 
	0, 103, 103, 81, 84, 0, 97, 96, 
	0, 100, 100, 101, 104, 96, 102, 0, 
	105, 0, 100, 100, 101, 105, 102, 0, 
	107, 106, 1, 2, 1, 3, 0, 108, 
	0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 76, 3, 4, 76, 26, 45, 
	5, 4, 5, 6, 7, 8, 10, 12, 
	13, 16, 8, 9, 11, 10, 76, 14, 
	15, 17, 17, 18, 22, 23, 18, 19, 
	21, 20, 10, 16, 20, 24, 25, 27, 
	29, 31, 32, 35, 27, 28, 30, 29, 
	76, 33, 34, 36, 36, 37, 41, 42, 
	37, 38, 40, 39, 29, 35, 39, 43, 
	44, 46, 47, 55, 48, 49, 48, 49, 
	50, 51, 52, 53, 54, 76, 76, 56, 
	57, 59, 61, 62, 65, 57, 58, 60, 
	59, 76, 63, 64, 66, 66, 67, 71, 
	72, 67, 68, 70, 69, 59, 65, 69, 
	73, 74, 75, 77, 0
};

static const char _command_trans_actions_wi[] = {
	17, 0, 15, 1, 3, 40, 5, 3, 
	3, 0, 0, 1, 9, 0, 0, 1, 
	1, 1, 1, 0, 1, 21, 27, 1, 
	1, 7, 0, 0, 1, 1, 1, 0, 
	1, 24, 24, 44, 0, 1, 1, 0, 
	0, 1, 1, 1, 1, 0, 1, 21, 
	33, 1, 1, 7, 0, 0, 1, 1, 
	1, 0, 1, 24, 24, 44, 0, 1, 
	1, 1, 1, 1, 13, 13, 0, 0, 
	0, 1, 1, 11, 1, 48, 36, 5, 
	0, 0, 1, 1, 1, 1, 0, 1, 
	21, 30, 1, 1, 7, 0, 0, 1, 
	1, 1, 0, 1, 24, 24, 44, 0, 
	1, 1, 0, 19, 0
};

static const int command_start = 1;
static const int command_first_final = 76;

static const int command_en_eat_line = 75;
static const int command_en_main = 1;

#line 12 "classes/command.rl"


Command::Command(Rubyk * pServer)
{
  int cs;
  mAction = NO_ACTION;
  mServer = pServer;
  mThread = 0;
  mQuit   = false;
  
#line 208 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 22 "classes/command.rl"
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
  

  
#line 260 "classes/command.cpp"
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
#line 69 "classes/command.rl"
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
#line 82 "classes/command.rl"
	{
      set_from_token(mVariable);
    }
	break;
	case 2:
#line 86 "classes/command.rl"
	{
      set_from_token(mMethod);
    }
	break;
	case 3:
#line 90 "classes/command.rl"
	{
      set_from_token(mKey);
    }
	break;
	case 4:
#line 94 "classes/command.rl"
	{
      set_class_from_token();
    }
	break;
	case 5:
#line 98 "classes/command.rl"
	{
      set_from_token(mValue);
    }
	break;
	case 6:
#line 102 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 7:
#line 108 "classes/command.rl"
	{
      set_from_token(mTo);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 113 "classes/command.rl"
	{
      create_instance();
    }
	break;
	case 9:
#line 117 "classes/command.rl"
	{
      set_single_param_from_token();
    }
	break;
	case 10:
#line 121 "classes/command.rl"
	{
      set_parameter(mKey, mValue);
    }
	break;
	case 11:
#line 125 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 131 "classes/command.rl"
	{
      execute_method();
    }
	break;
	case 13:
#line 135 "classes/command.rl"
	{
      execute_command();
    }
	break;
	case 14:
#line 143 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
	case 15:
#line 149 "classes/command.rl"
	{
      p--; // move back one char
      *mOutput << "Syntax error !" << std::endl;
      clear();
      prompt();
      {cs = 75; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 16:
#line 157 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 455 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 191 "classes/command.rl"

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
  Node * node = mServer->create_instance(mVariable, mClass, mParameters);
#ifdef DEBUG_PARSER
  std::cout << "NEW("<< mVariable << ", " << mClass << ", " << mParameters << ")";
#endif
  if (node) {
    *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
}


void Command::create_link()
{  
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  *mOutput << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node = mServer->get_instance(mVariable);
  if (node) {
    node->execute_method(mMethod, mParameters, mOutput);
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
}

// FIXME: execute_command should run in server space with concurrency locks.
void Command::execute_command()
{
  Node * node = mServer->get_instance(mMethod);
  // FIXME: 
  if (node) {
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
