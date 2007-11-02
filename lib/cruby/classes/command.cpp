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
	14, 2, 5, 9, 2, 5, 10, 2, 
	8, 14, 2, 12, 14, 2, 13, 14, 
	3, 2, 12, 14, 3, 2, 13, 14, 
	3, 5, 10, 0, 4, 1, 7, 11, 
	14
};

static const short _command_key_offsets[] = {
	0, 0, 5, 8, 21, 24, 28, 36, 
	42, 44, 45, 46, 46, 50, 52, 55, 
	63, 68, 70, 75, 80, 80, 88, 90, 
	97, 103, 105, 106, 107, 107, 111, 113, 
	116, 124, 129, 131, 136, 141, 141, 149, 
	151, 158, 162, 167, 170, 171, 175, 178, 
	180, 188, 197, 203, 205, 206, 207, 207, 
	211, 213, 216, 224, 229, 231, 236, 241, 
	241, 249, 251, 258
};

static const char _command_trans_keys[] = {
	9, 10, 32, 97, 122, 9, 10, 32, 
	9, 10, 32, 40, 46, 61, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 61, 
	9, 32, 65, 90, 40, 95, 48, 57, 
	65, 90, 97, 122, 34, 41, 49, 57, 
	97, 122, 34, 92, 41, 10, 41, 46, 
	48, 57, 48, 57, 41, 48, 57, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 34, 49, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 97, 122, 
	9, 32, 41, 46, 48, 57, 97, 122, 
	48, 57, 9, 32, 41, 48, 57, 97, 
	122, 34, 41, 49, 57, 97, 122, 34, 
	92, 41, 10, 41, 46, 48, 57, 48, 
	57, 41, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 9, 32, 34, 49, 
	57, 34, 92, 9, 32, 41, 97, 122, 
	9, 32, 41, 97, 122, 9, 32, 41, 
	46, 48, 57, 97, 122, 48, 57, 9, 
	32, 41, 48, 57, 97, 122, 49, 57, 
	97, 122, 9, 32, 61, 48, 57, 9, 
	32, 61, 62, 9, 32, 49, 57, 46, 
	48, 57, 97, 122, 10, 95, 48, 57, 
	65, 90, 97, 122, 10, 40, 95, 48, 
	57, 65, 90, 97, 122, 34, 41, 49, 
	57, 97, 122, 34, 92, 41, 10, 41, 
	46, 48, 57, 48, 57, 41, 48, 57, 
	58, 95, 48, 57, 65, 90, 97, 122, 
	9, 32, 34, 49, 57, 34, 92, 9, 
	32, 41, 97, 122, 9, 32, 41, 97, 
	122, 9, 32, 41, 46, 48, 57, 97, 
	122, 48, 57, 9, 32, 41, 48, 57, 
	97, 122, 9, 10, 32, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 7, 3, 2, 2, 2, 
	2, 1, 1, 0, 2, 0, 1, 2, 
	3, 2, 3, 3, 0, 4, 0, 3, 
	2, 2, 1, 1, 0, 2, 0, 1, 
	2, 3, 2, 3, 3, 0, 4, 0, 
	3, 0, 3, 3, 1, 2, 1, 0, 
	2, 3, 2, 2, 1, 1, 0, 2, 
	0, 1, 2, 3, 2, 3, 3, 0, 
	4, 0, 3, 3
};

static const char _command_range_lengths[] = {
	0, 1, 0, 3, 0, 1, 3, 2, 
	0, 0, 0, 0, 1, 1, 1, 3, 
	1, 0, 1, 1, 0, 2, 1, 2, 
	2, 0, 0, 0, 0, 1, 1, 1, 
	3, 1, 0, 1, 1, 0, 2, 1, 
	2, 2, 1, 0, 0, 1, 1, 1, 
	3, 3, 2, 0, 0, 0, 0, 1, 
	1, 1, 3, 1, 0, 1, 1, 0, 
	2, 1, 2, 1
};

static const short _command_index_offsets[] = {
	0, 0, 5, 9, 20, 24, 28, 34, 
	39, 42, 44, 46, 47, 51, 53, 56, 
	62, 67, 70, 75, 80, 81, 88, 90, 
	96, 101, 104, 106, 108, 109, 113, 115, 
	118, 124, 129, 132, 137, 142, 143, 150, 
	152, 158, 161, 166, 170, 172, 176, 179, 
	181, 187, 194, 199, 202, 204, 206, 207, 
	211, 213, 216, 222, 227, 230, 235, 240, 
	241, 248, 250, 256
};

static const char _command_indicies[] = {
	0, 2, 0, 3, 1, 0, 2, 0, 
	1, 4, 5, 4, 6, 7, 8, 3, 
	3, 3, 3, 1, 9, 9, 10, 1, 
	10, 10, 11, 1, 12, 11, 11, 11, 
	11, 1, 13, 14, 15, 16, 1, 18, 
	19, 17, 20, 1, 21, 1, 17, 20, 
	22, 15, 1, 23, 1, 20, 23, 1, 
	24, 16, 16, 16, 16, 1, 25, 25, 
	26, 27, 1, 29, 30, 28, 31, 31, 
	32, 33, 1, 34, 34, 14, 16, 1, 
	28, 31, 31, 32, 35, 27, 33, 1, 
	36, 1, 31, 31, 32, 36, 33, 1, 
	37, 38, 39, 40, 1, 42, 43, 41, 
	44, 1, 45, 1, 41, 44, 46, 39, 
	1, 47, 1, 44, 47, 1, 48, 40, 
	40, 40, 40, 1, 49, 49, 50, 51, 
	1, 53, 54, 52, 55, 55, 56, 57, 
	1, 58, 58, 38, 40, 1, 52, 55, 
	55, 56, 59, 51, 57, 1, 60, 1, 
	55, 55, 56, 60, 57, 1, 61, 62, 
	1, 63, 63, 64, 61, 1, 65, 65, 
	66, 1, 67, 1, 67, 67, 68, 1, 
	69, 68, 1, 70, 1, 71, 70, 70, 
	70, 70, 1, 72, 73, 62, 62, 62, 
	62, 1, 74, 75, 76, 77, 1, 79, 
	80, 78, 81, 1, 82, 1, 78, 81, 
	83, 76, 1, 84, 1, 81, 84, 1, 
	85, 77, 77, 77, 77, 1, 86, 86, 
	87, 88, 1, 90, 91, 89, 92, 92, 
	93, 94, 1, 95, 95, 75, 77, 1, 
	89, 92, 92, 93, 96, 88, 94, 1, 
	97, 1, 92, 92, 93, 97, 94, 1, 
	0, 2, 0, 3, 1, 0
};

static const char _command_trans_targs_wi[] = {
	2, 0, 67, 3, 4, 67, 24, 41, 
	5, 4, 5, 6, 7, 8, 10, 12, 
	15, 8, 9, 11, 10, 67, 13, 14, 
	16, 16, 17, 21, 17, 18, 20, 19, 
	10, 15, 19, 22, 23, 25, 27, 29, 
	32, 25, 26, 28, 27, 67, 30, 31, 
	33, 33, 34, 38, 34, 35, 37, 36, 
	27, 32, 36, 39, 40, 42, 49, 43, 
	44, 43, 44, 45, 46, 47, 48, 67, 
	67, 50, 51, 53, 55, 58, 51, 52, 
	54, 53, 67, 56, 57, 59, 59, 60, 
	64, 60, 61, 63, 62, 53, 58, 62, 
	65, 66
};

static const char _command_trans_actions_wi[] = {
	0, 0, 15, 1, 3, 36, 5, 3, 
	3, 0, 0, 1, 9, 0, 0, 1, 
	1, 1, 0, 1, 17, 23, 1, 1, 
	7, 0, 0, 1, 1, 0, 1, 20, 
	20, 40, 0, 1, 1, 0, 0, 1, 
	1, 1, 0, 1, 17, 29, 1, 1, 
	7, 0, 0, 1, 1, 0, 1, 20, 
	20, 40, 0, 1, 1, 1, 1, 13, 
	13, 0, 0, 0, 1, 11, 1, 44, 
	32, 5, 0, 0, 1, 1, 1, 0, 
	1, 17, 26, 1, 1, 7, 0, 0, 
	1, 1, 0, 1, 20, 20, 40, 0, 
	1, 1
};

static const int command_start = 1;
static const int command_first_final = 67;

static const int command_en_main = 1;

#line 12 "classes/command.rl"


Command::Command(Rubyk * pServer)
{
  int cs;
  mAction = NO_ACTION;
  mServer = pServer;
  mThread = 0;
  mQuit   = false;
  
#line 191 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 22 "classes/command.rl"
  mCurrentState = cs;
  mTokenIndex = 0;
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

void Command::do_listen()
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
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs = mCurrentState;        // restore machine state
  

  
#line 240 "classes/command.cpp"
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
#line 66 "classes/command.rl"
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
#line 79 "classes/command.rl"
	{
      set_from_token(mVariable);
    }
	break;
	case 2:
#line 83 "classes/command.rl"
	{
      set_from_token(mMethod);
    }
	break;
	case 3:
#line 87 "classes/command.rl"
	{
      set_from_token(mKey);
    }
	break;
	case 4:
#line 91 "classes/command.rl"
	{
      set_class_from_token();
    }
	break;
	case 5:
#line 95 "classes/command.rl"
	{
      set_from_token(mValue);
    }
	break;
	case 6:
#line 99 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 7:
#line 105 "classes/command.rl"
	{
      set_from_token(mTo);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 110 "classes/command.rl"
	{
      create_instance();
    }
	break;
	case 9:
#line 114 "classes/command.rl"
	{
      set_single_param_from_token();
    }
	break;
	case 10:
#line 118 "classes/command.rl"
	{
      set_parameter(mKey, mValue);
    }
	break;
	case 11:
#line 122 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 128 "classes/command.rl"
	{
      execute_method();
    }
	break;
	case 13:
#line 132 "classes/command.rl"
	{
      execute_command();
    }
	break;
	case 14:
#line 140 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
#line 421 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 179 "classes/command.rl"

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
  if (mClass != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
  }
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
    node->execute_method(mMethod, mParameters);
    *mOutput << node->inspect() << std::endl;
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
