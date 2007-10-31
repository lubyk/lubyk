#line 1 "classes/command.rl"
#include "command.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include "rubyk.h"


#line 10 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 6, 1, 8, 2, 
	0, 5, 2, 0, 7, 2, 0, 9, 
	2, 4, 6, 2, 4, 7, 2, 4, 
	8, 2, 8, 0, 2, 8, 6, 3, 
	0, 1, 9, 3, 4, 8, 0, 3, 
	4, 8, 6
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 2, 14, 18, 23, 31, 37, 
	39, 40, 40, 44, 46, 49, 57, 63, 
	65, 71, 77, 77, 86, 88, 96, 98, 
	104, 108, 109, 114, 117, 119, 124, 134, 
	146
};

static const char _command_trans_keys[] = {
	97, 122, 32, 46, 61, 95, 9, 10, 
	48, 57, 65, 90, 97, 122, 32, 61, 
	9, 10, 32, 9, 10, 65, 90, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	41, 49, 57, 97, 122, 34, 92, 41, 
	41, 46, 48, 57, 48, 57, 41, 48, 
	57, 58, 95, 48, 57, 65, 90, 97, 
	122, 32, 34, 9, 10, 49, 57, 34, 
	92, 32, 41, 9, 10, 97, 122, 32, 
	41, 9, 10, 97, 122, 32, 41, 46, 
	9, 10, 48, 57, 97, 122, 48, 57, 
	32, 41, 9, 10, 48, 57, 97, 122, 
	49, 57, 32, 61, 9, 10, 48, 57, 
	32, 61, 9, 10, 62, 32, 9, 10, 
	49, 57, 46, 48, 57, 97, 122, 32, 
	9, 10, 97, 122, 32, 95, 9, 10, 
	48, 57, 65, 90, 97, 122, 32, 46, 
	61, 95, 9, 10, 48, 57, 65, 90, 
	97, 122, 32, 61, 9, 10, 97, 122, 
	0
};

static const char _command_single_lengths[] = {
	0, 0, 4, 2, 1, 2, 2, 2, 
	1, 0, 2, 0, 1, 2, 2, 2, 
	2, 2, 0, 3, 0, 2, 0, 2, 
	2, 1, 1, 1, 0, 1, 2, 4, 
	2
};

static const char _command_range_lengths[] = {
	0, 1, 4, 1, 2, 3, 2, 0, 
	0, 0, 1, 1, 1, 3, 2, 0, 
	2, 2, 0, 3, 1, 3, 1, 2, 
	1, 0, 2, 1, 1, 2, 4, 4, 
	2
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 2, 11, 15, 19, 25, 30, 
	33, 35, 36, 40, 42, 45, 51, 56, 
	59, 64, 69, 70, 77, 79, 85, 87, 
	92, 96, 98, 102, 105, 107, 111, 118, 
	127
};

static const char _command_indicies[] = {
	0, 1, 2, 3, 4, 0, 2, 0, 
	0, 0, 1, 5, 6, 5, 1, 6, 
	6, 7, 1, 8, 7, 7, 7, 7, 
	1, 9, 10, 11, 12, 1, 14, 15, 
	13, 10, 1, 13, 16, 17, 11, 1, 
	18, 1, 16, 18, 1, 19, 12, 12, 
	12, 12, 1, 20, 21, 20, 22, 1, 
	24, 25, 23, 26, 27, 26, 28, 1, 
	29, 10, 29, 12, 1, 23, 30, 31, 
	32, 30, 22, 33, 1, 34, 1, 30, 
	31, 30, 34, 33, 1, 35, 1, 36, 
	37, 36, 35, 1, 38, 39, 38, 1, 
	40, 1, 40, 40, 41, 1, 42, 41, 
	1, 43, 1, 44, 44, 0, 1, 45, 
	43, 45, 43, 43, 46, 1, 47, 3, 
	4, 48, 47, 48, 48, 46, 1, 49, 
	6, 49, 0, 1, 0
};

static const char _command_trans_targs_wi[] = {
	2, 0, 3, 22, 4, 3, 4, 5, 
	6, 7, 29, 10, 13, 7, 8, 9, 
	29, 11, 12, 14, 14, 15, 19, 15, 
	16, 18, 17, 29, 13, 17, 17, 29, 
	20, 13, 21, 23, 24, 25, 24, 25, 
	26, 27, 28, 30, 29, 29, 31, 32, 
	31, 32
};

static const char _command_trans_actions_wi[] = {
	1, 0, 3, 3, 3, 0, 0, 1, 
	7, 0, 11, 18, 1, 1, 27, 0, 
	24, 1, 18, 5, 0, 0, 1, 1, 
	9, 0, 13, 36, 33, 0, 30, 47, 
	1, 43, 1, 15, 9, 9, 0, 0, 
	0, 1, 9, 21, 0, 3, 39, 3, 
	21, 0
};

static const int command_start = 1;
static const int command_first_final = 29;

static const int command_en_main = 1;

#line 10 "classes/command.rl"


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
  std::string iss;
  *mOutput << "Welcome to rubyk !\n\n";
  
  while(!mQuit && !mInput->eof()) {
    *mInput >> iss;
    parse(iss);
  }
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  int cs;        // machine state
  
  mTokenIndex = 0;

  
#line 157 "classes/command.cpp"
	{
	cs = command_start;
	}

#line 162 "classes/command.cpp"
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
#line 45 "classes/command.rl"
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
#line 55 "classes/command.rl"
	{
      setFromToken(mVariable);
    }
	break;
	case 2:
#line 59 "classes/command.rl"
	{
      setFromToken(mKey);
    }
	break;
	case 3:
#line 63 "classes/command.rl"
	{
      setClassFromToken();
    }
	break;
	case 4:
#line 75 "classes/command.rl"
	{
      setFromToken(mValue);
    }
	break;
	case 5:
#line 79 "classes/command.rl"
	{
      mFromPort = atoi(mValue.c_str());
      mFrom = mVariable;
    }
	break;
	case 6:
#line 84 "classes/command.rl"
	{
      //std::cout << "NEW (" << mVariable << " = " << mClass << "(" << mParameters << ")" << ")" << std::endl;
      createInstance();
    }
	break;
	case 7:
#line 89 "classes/command.rl"
	{
      setSingleParamFromToken();
    }
	break;
	case 8:
#line 93 "classes/command.rl"
	{
      setParameter(mKey, mValue);
    }
	break;
	case 9:
#line 97 "classes/command.rl"
	{
      mToPort = atoi(mValue.c_str());
      mTo   = mVariable;
      *mOutput << "LINK (" << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << ")" << std::endl;
    }
	break;
#line 307 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}
#line 131 "classes/command.rl"

}


void Command::close() {
  mQuit = true;
  // FIXME: how to force out of input (>>) ?
  if (mThread)
    pthread_join( mThread, NULL); // wait for listen to finish
}

void Command::setFromToken (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
  //std::cout << "[" << mToken << "]" << std::endl;
  pElem = mToken;
  mTokenIndex = 0;
}


void Command::setClassFromToken  () 
{
  setFromToken(mClass);
  if (mSingleParam != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
    mSingleParam = "";
  }
}

void Command::setSingleParamFromToken () 
{
  setFromToken(mSingleParam);
  if (mClass != "") {
    std::string key = mClass;
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    mParameters.set(key, mSingleParam);
  }
}

void Command::setParameter  (const std::string& pKey, const std::string& pValue) 
{
	mParameters.set(pKey,pValue);
}

void Command::createInstance()
{
  Node * node = mServer->create_instance(mVariable, mClass, mParameters);
  if (node) {
    *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
  clear();
}

void Command::printCommand(void)
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
