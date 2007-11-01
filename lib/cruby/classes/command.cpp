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
	3, 1, 4, 1, 5, 1, 13, 2, 
	0, 8, 2, 0, 9, 2, 5, 0, 
	2, 5, 6, 2, 5, 8, 2, 5, 
	9, 2, 7, 13, 2, 11, 13, 2, 
	12, 13, 3, 1, 10, 13, 3, 2, 
	11, 13, 3, 2, 12, 13
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 5, 8, 21, 24, 28, 36, 
	42, 44, 45, 46, 46, 50, 52, 55, 
	63, 68, 70, 75, 75, 83, 85, 92, 
	98, 100, 101, 102, 102, 106, 108, 111, 
	119, 124, 126, 131, 131, 139, 141, 148, 
	152, 157, 160, 161, 165, 168, 170, 178, 
	187, 193, 195, 196, 197, 197, 201, 203, 
	206, 214, 219, 221, 226, 226, 234, 236, 
	243
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
	41, 97, 122, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 34, 41, 49, 57, 
	97, 122, 34, 92, 41, 10, 41, 46, 
	48, 57, 48, 57, 41, 48, 57, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 34, 49, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 46, 48, 
	57, 97, 122, 48, 57, 9, 32, 41, 
	48, 57, 97, 122, 49, 57, 97, 122, 
	9, 32, 61, 48, 57, 9, 32, 61, 
	62, 9, 32, 49, 57, 46, 48, 57, 
	97, 122, 10, 95, 48, 57, 65, 90, 
	97, 122, 10, 40, 95, 48, 57, 65, 
	90, 97, 122, 34, 41, 49, 57, 97, 
	122, 34, 92, 41, 10, 41, 46, 48, 
	57, 48, 57, 41, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	34, 49, 57, 34, 92, 9, 32, 41, 
	97, 122, 9, 32, 41, 46, 48, 57, 
	97, 122, 48, 57, 9, 32, 41, 48, 
	57, 97, 122, 9, 10, 32, 97, 122, 
	0
};

static const char _command_single_lengths[] = {
	0, 3, 3, 7, 3, 2, 2, 2, 
	2, 1, 1, 0, 2, 0, 1, 2, 
	3, 2, 3, 0, 4, 0, 3, 2, 
	2, 1, 1, 0, 2, 0, 1, 2, 
	3, 2, 3, 0, 4, 0, 3, 0, 
	3, 3, 1, 2, 1, 0, 2, 3, 
	2, 2, 1, 1, 0, 2, 0, 1, 
	2, 3, 2, 3, 0, 4, 0, 3, 
	3
};

static const char _command_range_lengths[] = {
	0, 1, 0, 3, 0, 1, 3, 2, 
	0, 0, 0, 0, 1, 1, 1, 3, 
	1, 0, 1, 0, 2, 1, 2, 2, 
	0, 0, 0, 0, 1, 1, 1, 3, 
	1, 0, 1, 0, 2, 1, 2, 2, 
	1, 0, 0, 1, 1, 1, 3, 3, 
	2, 0, 0, 0, 0, 1, 1, 1, 
	3, 1, 0, 1, 0, 2, 1, 2, 
	1
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 5, 9, 20, 24, 28, 34, 
	39, 42, 44, 46, 47, 51, 53, 56, 
	62, 67, 70, 75, 76, 83, 85, 91, 
	96, 99, 101, 103, 104, 108, 110, 113, 
	119, 124, 127, 132, 133, 140, 142, 148, 
	151, 156, 160, 162, 166, 169, 171, 177, 
	184, 189, 192, 194, 196, 197, 201, 203, 
	206, 212, 217, 220, 225, 226, 233, 235, 
	241
};

static const char _command_indicies[] = {
	0, 2, 0, 3, 1, 0, 2, 0, 
	1, 4, 5, 4, 6, 7, 8, 3, 
	3, 3, 3, 1, 9, 9, 10, 1, 
	10, 10, 11, 1, 12, 11, 11, 11, 
	11, 1, 13, 14, 15, 16, 1, 18, 
	19, 17, 14, 1, 20, 1, 17, 21, 
	22, 15, 1, 23, 1, 21, 23, 1, 
	24, 16, 16, 16, 16, 1, 25, 25, 
	26, 27, 1, 29, 30, 28, 31, 31, 
	14, 16, 1, 28, 32, 32, 21, 33, 
	27, 34, 1, 35, 1, 32, 32, 21, 
	35, 34, 1, 36, 37, 38, 39, 1, 
	41, 42, 40, 37, 1, 43, 1, 40, 
	44, 45, 38, 1, 46, 1, 44, 46, 
	1, 47, 39, 39, 39, 39, 1, 48, 
	48, 49, 50, 1, 52, 53, 51, 54, 
	54, 37, 39, 1, 51, 55, 55, 44, 
	56, 50, 57, 1, 58, 1, 55, 55, 
	44, 58, 57, 1, 59, 60, 1, 61, 
	61, 62, 59, 1, 63, 63, 64, 1, 
	65, 1, 65, 65, 66, 1, 67, 66, 
	1, 68, 1, 69, 68, 68, 68, 68, 
	1, 70, 71, 60, 60, 60, 60, 1, 
	72, 73, 74, 75, 1, 77, 78, 76, 
	73, 1, 79, 1, 76, 80, 81, 74, 
	1, 82, 1, 80, 82, 1, 83, 75, 
	75, 75, 75, 1, 84, 84, 85, 86, 
	1, 88, 89, 87, 90, 90, 73, 75, 
	1, 87, 91, 91, 80, 92, 86, 93, 
	1, 94, 1, 91, 91, 80, 94, 93, 
	1, 0, 2, 0, 3, 1, 0
};

static const char _command_trans_targs_wi[] = {
	2, 0, 64, 3, 4, 64, 23, 39, 
	5, 4, 5, 6, 7, 8, 10, 12, 
	15, 8, 9, 11, 64, 10, 13, 14, 
	16, 16, 17, 20, 17, 18, 19, 18, 
	18, 21, 15, 22, 24, 26, 28, 31, 
	24, 25, 27, 64, 26, 29, 30, 32, 
	32, 33, 36, 33, 34, 35, 34, 34, 
	37, 31, 38, 40, 47, 41, 42, 41, 
	42, 43, 44, 45, 46, 64, 64, 48, 
	49, 51, 53, 56, 49, 50, 52, 64, 
	51, 54, 55, 57, 57, 58, 61, 58, 
	59, 60, 59, 59, 62, 56, 63
};

static const char _command_trans_actions_wi[] = {
	0, 0, 13, 1, 3, 50, 5, 3, 
	3, 0, 0, 1, 9, 0, 0, 15, 
	1, 1, 27, 0, 33, 11, 1, 15, 
	7, 0, 0, 18, 1, 30, 0, 0, 
	11, 1, 21, 18, 0, 0, 15, 1, 
	1, 27, 0, 39, 11, 1, 15, 7, 
	0, 0, 18, 1, 30, 0, 0, 11, 
	1, 21, 18, 1, 1, 24, 24, 0, 
	0, 0, 1, 11, 1, 42, 46, 5, 
	0, 0, 15, 1, 1, 27, 0, 36, 
	11, 1, 15, 7, 0, 0, 18, 1, 
	30, 0, 0, 11, 1, 21, 18
};

static const int command_start = 1;
static const int command_first_final = 64;

static const int command_en_main = 1;

#line 12 "classes/command.rl"


Command::Command(Rubyk& pServer)
{
  int cs;
  mAction = NO_ACTION;
  mServer = &pServer;
  mThread = 0;
  mQuit   = false;
  
#line 186 "classes/command.cpp"
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

  while(!mQuit) {
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
  

  
#line 235 "classes/command.cpp"
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
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 7:
#line 104 "classes/command.rl"
	{
      create_instance();
    }
	break;
	case 8:
#line 108 "classes/command.rl"
	{
      set_single_param_from_token();
    }
	break;
	case 9:
#line 112 "classes/command.rl"
	{
      set_parameter(mKey, mValue);
    }
	break;
	case 10:
#line 116 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
	break;
	case 11:
#line 122 "classes/command.rl"
	{
      execute_method();
    }
	break;
	case 12:
#line 126 "classes/command.rl"
	{
      execute_command();
    }
	break;
	case 13:
#line 134 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
        prompt();
      }
    }
	break;
#line 408 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 173 "classes/command.rl"

    
#ifdef DEBUG_PARSER
  printf("{%i}",cs);
#endif
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
  std::cout << "[" << mTokenIndex << ":" << mToken << "]" << std::endl;
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
  set_from_token(mSingleParam);
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

void Command::create_instance()
{
  Node * node = mServer->create_instance(mVariable, mClass, mParameters);
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
