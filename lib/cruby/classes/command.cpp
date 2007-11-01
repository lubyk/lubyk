#line 1 "classes/command.rl"
#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"


#line 10 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 9, 1, 
	12, 1, 13, 2, 0, 6, 2, 0, 
	8, 2, 1, 12, 2, 1, 13, 2, 
	5, 8, 2, 5, 9, 2, 7, 13, 
	2, 9, 0, 2, 11, 13, 2, 12, 
	13, 3, 0, 10, 13, 3, 1, 12, 
	13, 3, 5, 7, 13, 3, 5, 9, 
	0, 3, 5, 11, 13, 3, 9, 7, 
	13, 3, 9, 11, 13, 4, 0, 1, 
	10, 13, 4, 5, 9, 7, 13, 4, 
	5, 9, 11, 13
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 5, 17, 21, 26, 34, 40, 
	42, 43, 43, 47, 49, 52, 60, 66, 
	68, 74, 80, 80, 89, 91, 99, 103, 
	109, 113, 114, 119, 122, 124, 132, 138, 
	140, 141, 141, 145, 147, 150, 158, 164, 
	166, 172, 178, 178, 187, 189, 197, 202, 
	212, 224
};

static const char _command_trans_keys[] = {
	32, 9, 10, 97, 122, 32, 46, 61, 
	95, 9, 10, 48, 57, 65, 90, 97, 
	122, 32, 61, 9, 10, 32, 9, 10, 
	65, 90, 40, 95, 48, 57, 65, 90, 
	97, 122, 34, 41, 49, 57, 97, 122, 
	34, 92, 41, 41, 46, 48, 57, 48, 
	57, 41, 48, 57, 58, 95, 48, 57, 
	65, 90, 97, 122, 32, 34, 9, 10, 
	49, 57, 34, 92, 32, 41, 9, 10, 
	97, 122, 32, 41, 9, 10, 97, 122, 
	32, 41, 46, 9, 10, 48, 57, 97, 
	122, 48, 57, 32, 41, 9, 10, 48, 
	57, 97, 122, 49, 57, 97, 122, 32, 
	61, 9, 10, 48, 57, 32, 61, 9, 
	10, 62, 32, 9, 10, 49, 57, 46, 
	48, 57, 97, 122, 40, 95, 48, 57, 
	65, 90, 97, 122, 34, 41, 49, 57, 
	97, 122, 34, 92, 41, 41, 46, 48, 
	57, 48, 57, 41, 48, 57, 58, 95, 
	48, 57, 65, 90, 97, 122, 32, 34, 
	9, 10, 49, 57, 34, 92, 32, 41, 
	9, 10, 97, 122, 32, 41, 9, 10, 
	97, 122, 32, 41, 46, 9, 10, 48, 
	57, 97, 122, 48, 57, 32, 41, 9, 
	10, 48, 57, 97, 122, 32, 9, 10, 
	97, 122, 32, 95, 9, 10, 48, 57, 
	65, 90, 97, 122, 32, 46, 61, 95, 
	9, 10, 48, 57, 65, 90, 97, 122, 
	32, 61, 9, 10, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 1, 4, 2, 1, 2, 2, 2, 
	1, 0, 2, 0, 1, 2, 2, 2, 
	2, 2, 0, 3, 0, 2, 0, 2, 
	2, 1, 1, 1, 0, 2, 2, 2, 
	1, 0, 2, 0, 1, 2, 2, 2, 
	2, 2, 0, 3, 0, 2, 1, 2, 
	4, 2
};

static const char _command_range_lengths[] = {
	0, 2, 4, 1, 2, 3, 2, 0, 
	0, 0, 1, 1, 1, 3, 2, 0, 
	2, 2, 0, 3, 1, 3, 2, 2, 
	1, 0, 2, 1, 1, 3, 2, 0, 
	0, 0, 1, 1, 1, 3, 2, 0, 
	2, 2, 0, 3, 1, 3, 2, 4, 
	4, 2
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 4, 13, 17, 21, 27, 32, 
	35, 37, 38, 42, 44, 47, 53, 58, 
	61, 66, 71, 72, 79, 81, 87, 90, 
	95, 99, 101, 105, 108, 110, 116, 121, 
	124, 126, 127, 131, 133, 136, 142, 147, 
	150, 155, 160, 161, 168, 170, 176, 180, 
	187, 196
};

static const char _command_indicies[] = {
	0, 0, 2, 1, 3, 4, 5, 2, 
	3, 2, 2, 2, 1, 6, 7, 6, 
	1, 8, 8, 9, 1, 10, 9, 9, 
	9, 9, 1, 11, 12, 13, 14, 1, 
	16, 17, 15, 12, 1, 15, 18, 19, 
	13, 1, 20, 1, 18, 20, 1, 21, 
	14, 14, 14, 14, 1, 22, 23, 22, 
	24, 1, 26, 27, 25, 28, 29, 28, 
	30, 1, 31, 12, 31, 14, 1, 25, 
	32, 33, 34, 32, 24, 35, 1, 36, 
	1, 32, 33, 32, 36, 35, 1, 37, 
	38, 1, 39, 40, 39, 37, 1, 41, 
	42, 41, 1, 43, 1, 43, 43, 44, 
	1, 45, 44, 1, 46, 1, 47, 38, 
	38, 38, 38, 1, 48, 49, 50, 51, 
	1, 53, 54, 52, 49, 1, 52, 55, 
	56, 50, 1, 57, 1, 55, 57, 1, 
	58, 51, 51, 51, 51, 1, 59, 60, 
	59, 61, 1, 63, 64, 62, 65, 66, 
	65, 67, 1, 68, 49, 68, 51, 1, 
	62, 69, 70, 71, 69, 61, 72, 1, 
	73, 1, 69, 70, 69, 73, 72, 1, 
	0, 0, 2, 1, 74, 46, 74, 46, 
	46, 75, 1, 76, 4, 5, 77, 76, 
	77, 77, 75, 1, 78, 7, 78, 2, 
	1, 0
};

static const char _command_trans_targs_wi[] = {
	46, 0, 2, 3, 22, 4, 3, 4, 
	4, 5, 6, 7, 46, 10, 13, 7, 
	8, 9, 46, 11, 12, 14, 14, 15, 
	19, 15, 16, 18, 17, 46, 13, 17, 
	17, 46, 20, 13, 21, 23, 29, 24, 
	25, 24, 25, 26, 27, 28, 47, 30, 
	31, 46, 34, 37, 31, 32, 33, 46, 
	35, 36, 38, 38, 39, 43, 39, 40, 
	42, 41, 46, 37, 41, 41, 46, 44, 
	37, 45, 46, 48, 49, 48, 49
};

static const char _command_trans_actions_wi[] = {
	17, 0, 1, 25, 3, 25, 15, 15, 
	0, 1, 9, 0, 37, 22, 1, 1, 
	31, 0, 57, 1, 22, 7, 0, 0, 
	1, 1, 11, 0, 13, 69, 40, 0, 
	34, 82, 1, 61, 1, 19, 1, 11, 
	11, 0, 0, 0, 1, 11, 49, 5, 
	0, 43, 22, 1, 1, 31, 0, 65, 
	1, 22, 7, 0, 0, 1, 1, 11, 
	0, 13, 73, 40, 0, 34, 87, 1, 
	61, 1, 28, 77, 53, 49, 46
};

static const int command_start = 1;
static const int command_first_final = 46;

static const int command_en_main = 1;

#line 10 "classes/command.rl"


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
  }
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs = mCurrentState;        // restore machine state
  
  mTokenIndex = 0;

  
#line 201 "classes/command.cpp"
	{
	cs = command_start;
	}

#line 206 "classes/command.cpp"
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
#line 52 "classes/command.rl"
	{
      if (mTokenIndex >= MAX_TOKEN_SIZE) {
        std::cerr << "Buffer overflow !" << std::endl;
        // stop parsing
        return;
      }
      
      mToken[mTokenIndex] = (*p); /* append */
      mTokenIndex++;     
    }
	break;
	case 1:
#line 63 "classes/command.rl"
	{
      set_from_token(mVariable);
    }
	break;
	case 2:
#line 67 "classes/command.rl"
	{
      set_from_token(mMethod);
    }
	break;
	case 3:
#line 71 "classes/command.rl"
	{
      set_from_token(mKey);
    }
	break;
	case 4:
#line 75 "classes/command.rl"
	{
      set_class_from_token();
    }
	break;
	case 5:
#line 79 "classes/command.rl"
	{
      set_from_token(mValue);
    }
	break;
	case 6:
#line 83 "classes/command.rl"
	{
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 7:
#line 88 "classes/command.rl"
	{
      create_instance();
    }
	break;
	case 8:
#line 92 "classes/command.rl"
	{
      set_single_param_from_token();
    }
	break;
	case 9:
#line 96 "classes/command.rl"
	{
      set_parameter(mKey, mValue);
    }
	break;
	case 10:
#line 100 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      create_link();
    }
	break;
	case 11:
#line 106 "classes/command.rl"
	{
      execute_method();
    }
	break;
	case 12:
#line 114 "classes/command.rl"
	{
      printf("[%i]", cs);
    }
	break;
	case 13:
#line 118 "classes/command.rl"
	{
      clear();
      prompt();
    }
	break;
#line 375 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 155 "classes/command.rl"

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
  std::cout << "[" << mToken << "]" << std::endl;
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
  *mOutput << "LINK (" << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << ")" << std::endl;
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
