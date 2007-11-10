#line 1 "objects/Turing.rl"
#include "script.h"
#define MAX_NAME_SIZE 200


#line 7 "objects/Turing.cpp"
static const char _turing_actions[] = {
	0, 1, 0, 1, 4, 1, 5, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 2, 1, 2, 2, 1, 3, 2, 
	6, 7, 3, 1, 2, 5, 3, 1, 
	3, 6, 4, 0, 1, 2, 4, 4, 
	0, 1, 3, 4
};

static const unsigned char _turing_key_offsets[] = {
	0, 0, 9, 10, 11, 12, 13, 14, 
	15, 16, 26, 34, 38, 39, 42, 45, 
	47, 53, 63, 71, 75, 76, 80, 84, 
	85, 93, 97, 98, 101, 104, 116, 126, 
	135, 142, 152, 154, 155, 157, 159, 161, 
	162, 171
};

static const char _turing_trans_keys[] = {
	9, 10, 32, 35, 61, 65, 90, 97, 
	122, 10, 98, 101, 103, 105, 110, 10, 
	9, 32, 39, 45, 48, 57, 65, 90, 
	97, 122, 9, 32, 39, 45, 65, 90, 
	97, 122, 65, 90, 97, 122, 39, 9, 
	32, 45, 9, 32, 45, 45, 62, 9, 
	32, 65, 90, 97, 122, 9, 32, 39, 
	123, 48, 57, 65, 90, 97, 122, 9, 
	32, 39, 123, 65, 90, 97, 122, 65, 
	90, 97, 122, 39, 9, 10, 32, 35, 
	9, 10, 32, 35, 10, 9, 32, 39, 
	125, 65, 90, 97, 122, 65, 90, 97, 
	122, 39, 9, 32, 125, 9, 32, 125, 
	9, 10, 32, 35, 39, 123, 48, 57, 
	65, 90, 97, 122, 9, 10, 32, 35, 
	39, 123, 65, 90, 97, 122, 9, 32, 
	39, 45, 62, 65, 90, 97, 122, 9, 
	32, 39, 65, 90, 97, 122, 9, 32, 
	39, 45, 48, 57, 65, 90, 97, 122, 
	10, 61, 10, 10, 101, 10, 110, 10, 
	100, 10, 9, 10, 32, 35, 61, 65, 
	90, 97, 122, 10, 61, 0
};

static const char _turing_single_lengths[] = {
	0, 5, 1, 1, 1, 1, 1, 1, 
	1, 4, 4, 0, 1, 3, 3, 2, 
	2, 4, 4, 0, 1, 4, 4, 1, 
	4, 0, 1, 3, 3, 6, 6, 5, 
	3, 4, 2, 1, 2, 2, 2, 1, 
	5, 2
};

static const char _turing_range_lengths[] = {
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 3, 2, 2, 0, 0, 0, 0, 
	2, 3, 2, 2, 0, 0, 0, 0, 
	2, 2, 0, 0, 0, 3, 2, 2, 
	2, 3, 0, 0, 0, 0, 0, 0, 
	2, 0
};

static const unsigned char _turing_index_offsets[] = {
	0, 0, 8, 10, 12, 14, 16, 18, 
	20, 22, 30, 37, 40, 42, 46, 50, 
	53, 58, 66, 73, 76, 78, 83, 88, 
	90, 97, 100, 102, 106, 110, 120, 129, 
	137, 143, 151, 154, 156, 159, 162, 165, 
	167, 175
};

static const char _turing_indicies[] = {
	1, 2, 1, 3, 4, 5, 5, 0, 
	2, 3, 6, 0, 7, 0, 8, 0, 
	9, 0, 10, 0, 11, 0, 12, 12, 
	13, 14, 5, 15, 15, 0, 16, 16, 
	17, 18, 19, 19, 0, 20, 20, 0, 
	21, 0, 22, 22, 23, 0, 24, 24, 
	25, 0, 25, 26, 0, 26, 26, 27, 
	27, 0, 28, 28, 29, 31, 27, 30, 
	30, 0, 32, 32, 33, 35, 34, 34, 
	0, 36, 36, 0, 37, 0, 38, 39, 
	38, 40, 0, 41, 42, 41, 43, 0, 
	42, 43, 35, 35, 44, 41, 45, 45, 
	0, 46, 46, 0, 47, 0, 48, 48, 
	38, 0, 49, 49, 41, 0, 50, 39, 
	50, 40, 29, 31, 27, 30, 30, 0, 
	51, 42, 51, 43, 33, 35, 34, 34, 
	0, 52, 52, 17, 18, 26, 19, 19, 
	0, 52, 52, 17, 19, 19, 0, 53, 
	53, 13, 54, 5, 15, 15, 0, 56, 
	57, 55, 56, 55, 56, 58, 55, 56, 
	59, 55, 56, 60, 55, 61, 55, 1, 
	2, 1, 3, 4, 5, 5, 0, 56, 
	57, 55, 0
};

static const char _turing_trans_targs_wi[] = {
	0, 1, 40, 2, 3, 9, 4, 5, 
	6, 7, 8, 40, 10, 11, 31, 33, 
	10, 11, 31, 13, 12, 13, 14, 15, 
	14, 15, 16, 17, 18, 19, 29, 24, 
	18, 19, 21, 24, 20, 21, 22, 40, 
	23, 22, 40, 23, 25, 27, 26, 27, 
	28, 28, 30, 30, 32, 10, 31, 35, 
	41, 36, 37, 38, 39, 41
};

static const char _turing_trans_actions_wi[] = {
	11, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 13, 17, 17, 17, 34, 
	0, 0, 0, 3, 3, 0, 5, 5, 
	0, 0, 0, 1, 20, 20, 39, 20, 
	0, 0, 3, 0, 3, 0, 7, 23, 
	7, 0, 9, 0, 0, 3, 3, 0, 
	7, 0, 30, 0, 0, 26, 26, 0, 
	0, 0, 0, 0, 0, 15
};

static const char _turing_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 9, 9, 9, 
	0, 0, 0, 0, 0, 9, 9, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const int turing_start = 1;
static const int turing_first_final = 40;

static const int turing_en_doc_comment = 34;
static const int turing_en_main = 1;

#line 7 "objects/Turing.rl"


class Turing : public Script
{
public:
  ~Turing()
  {
    // free...
  }

  bool init (const Params& p)
  {
    mToken = 0;
    mState = 0;
    return init_script(p);
  }

  // inlet 1
  void input(const Signal& sig)
  { 
    int i = -1;
    sig.get(&i);
    if (i != -1) {
      mToken = mTokenTable[ i % 256 ]; // translate token in the current machine values.
    }
  }

  // outlet 1
  void output(Signal& sig)
  {
    int state;
    
    reload_script();
    if (mScriptDead) return;
    
    //std::cout << "goto\n";
    //print(std::cout, mGotoTable);
    //std::cout << "send\n";
    //print(std::cout, mSendTable);
    //
    //std::cout << "token ["<< mToken << "]" << std::endl;
    //std::cout << "state ["<< mState << "]" << std::endl;
    
    if (mSend = mSendTable[mState][mToken])
      ; // ok custom value
    else
      mSend = mSendTable[mState][0]; // use default

    if (state = mGotoTable[mState][mToken])
      mState = state;
    else
      mState = mGotoTable[mState][0]; // use default

    /* Send the value out. */
    if (mSend == -1)
      sig.set_nil();
    else
      sig.set(mSend);
  }


  void eval_script(const std::string& pScript) 
  {
    mScript = pScript;
    int cs;
    const char *p  = mScript.data(); // data pointer
    const char *pe = p + mScript.size(); // past end
    char name[MAX_NAME_SIZE + 1];
    int  name_index = 0;
    int state_count = 0; // first state is 0
    int token_count = 1; // first token is 1
    Hash<std::string, int> state_names(30);
    
    int state_id;
    int token_id;
    char tok;
    int send = -1;
    int source_state = 0;
    int target_state = 0;
    
    
    std::vector< std::vector<int> >::iterator it,end; // to add new tokens
    
    // init token table
    memset(mTokenTable, 0, sizeof(mTokenTable));
    
    mGotoTable.clear();
    mSendTable.clear();
    
    
#line 232 "objects/Turing.cpp"
	{
	cs = turing_start;
	}
#line 97 "objects/Turing.rl"
    
  
#line 239 "objects/Turing.cpp"
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
	_keys = _turing_trans_keys + _turing_key_offsets[cs];
	_trans = _turing_index_offsets[cs];

	_klen = _turing_single_lengths[cs];
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

	_klen = _turing_range_lengths[cs];
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
	_trans = _turing_indicies[_trans];
	cs = _turing_trans_targs_wi[_trans];

	if ( _turing_trans_actions_wi[_trans] == 0 )
		goto _again;

	_acts = _turing_actions + _turing_trans_actions_wi[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 99 "objects/Turing.rl"
	{
      if (name_index >= MAX_NAME_SIZE) {
        *mOutput << "Name buffer overflow !\n";
        mScriptDead = true;
        return;
      }
      #ifdef DEBUG_PARSER
        printf("_%c_",(*p));
      #endif
      name[name_index] = (*p); /* append */
      name_index++;     
    }
	break;
	case 1:
#line 112 "objects/Turing.rl"
	{ 
      name[name_index] = '\0';
      #ifdef DEBUG_PARSER
        std::cout <<    "[name " << name << "]" << std::endl;
      #endif
      // do we know this name ?
      if (!state_names.get(&state_id, std::string(name))) {
        // new state
        state_id = state_count;
        state_names.set(std::string(name), state_id);
        state_count++;
        
        // add a new line to the lookup tables
        mGotoTable.push_back( std::vector<int>(token_count, 0) );
        mSendTable.push_back( std::vector<int>(token_count, 0) );
      }
      name_index = 0;
    }
	break;
	case 2:
#line 131 "objects/Turing.rl"
	{ source_state = state_id; state_id = 0; }
	break;
	case 3:
#line 133 "objects/Turing.rl"
	{ target_state = state_id; state_id = 0; }
	break;
	case 4:
#line 135 "objects/Turing.rl"
	{ tok = (*p); }
	break;
	case 5:
#line 137 "objects/Turing.rl"
	{ 
      // FIXME: only works with letters, should also work with numbers
      // do we know this token ?
      if (!mTokenTable[tok % 256]) {
        // new token
        #ifdef DEBUG_PARSER
        printf("NEW TOKEN [%i] %c\n", token_count, tok);
        #endif
        
        mTokenTable[tok % 256] = token_count;
        token_count++;
        
        // enlarge lookup tables (add new column)
        end = mGotoTable.end();
        for (it = mGotoTable.begin(); it < end; it++) {
          // enlarge all arrays in the table
          (*it).push_back(0);
        }
        
        end = mSendTable.end();
        for (it = mSendTable.begin(); it < end; it++) {
          // enlarge all arrays in the table
          (*it).push_back(0);
        }
      }
      token_id = mTokenTable[tok % 256];
    }
	break;
	case 6:
#line 165 "objects/Turing.rl"
	{
      // write transition
      #ifdef DEBUG_PARSER
      printf("SEND[%c]\n",tok);
      #endif
      send = tok;
    }
	break;
	case 7:
#line 173 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      printf("ADD %i %i %i %c\n", source_state, token_id, target_state, send);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = -1;
      source_state = 0;
      target_state = 0;
    }
	break;
	case 8:
#line 188 "objects/Turing.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      mScriptDead = true;
      return;
    }
	break;
	case 9:
#line 202 "objects/Turing.rl"
	{ {cs = 34; goto _again;} }
	break;
	case 10:
#line 204 "objects/Turing.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 436 "objects/Turing.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}

#line 448 "objects/Turing.cpp"
	{
	const char *_acts = _turing_actions + _turing_eof_actions[cs];
	unsigned int _nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 7:
#line 173 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      printf("ADD %i %i %i %c\n", source_state, token_id, target_state, send);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = -1;
      source_state = 0;
      target_state = 0;
    }
	break;
#line 470 "objects/Turing.cpp"
		}
	}
	}

#line 227 "objects/Turing.rl"

    
    mScriptDead = false; // ok, we can receive and process signals (again).
  }

  void tables()
  {
    *mOutput << "goto\n";
    print(*mOutput, mGotoTable);
    *mOutput << "send\n";
    print(*mOutput, mSendTable);
  }

private:
  int  mToken;           /**< Current token value (translated). */
  int  mSend;            /**< Send result. */
  int  mState;           /**< Current state. */
  int  mTokenTable[256]; /**< Translate chars 'x', '3', etc into the value used in this state machine. */
  std::vector< std::vector<int> > mGotoTable; /**< State transition table. */
  std::vector< std::vector<int> > mSendTable; /**< State transition table. */
  
  
  void print(std::ostream& pOutput, std::vector< std::vector<int> >& pTable) {  
    std::vector< std::vector<int> >::iterator it,end;
    end = pTable.end();
    for (it = pTable.begin(); it < end; it++) {
      std::vector<int>::iterator it2,end2;
      end2 = (*it).end();
      for ( it2 = (*it).begin(); it2 < end2; it2++ ) {
        if (*it2 == -1)
          pOutput << " /";
        else
          pOutput << " " << *it2;
      }
      pOutput << "\n";
    } 
  }
};

extern "C" void init()
{
  CLASS (Turing)
  INLET (Turing, input)
  OUTLET(Turing, output)
  METHOD(Turing, tables)
  SUPER_METHOD(Turing, Script, set)
  SUPER_METHOD(Turing, Script, load)
  SUPER_METHOD(Turing, Script, script)
}
