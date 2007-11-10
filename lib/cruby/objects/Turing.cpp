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
	0, 0, 9, 12, 13, 14, 15, 16, 
	17, 18, 19, 29, 37, 41, 42, 45, 
	48, 50, 56, 66, 74, 78, 79, 83, 
	87, 88, 96, 100, 101, 104, 107, 119, 
	129, 138, 145, 155, 157, 158, 160, 162, 
	164, 165, 174
};

static const char _turing_trans_keys[] = {
	9, 10, 32, 35, 61, 65, 90, 97, 
	122, 9, 10, 32, 10, 98, 101, 103, 
	105, 110, 10, 9, 32, 39, 45, 48, 
	57, 65, 90, 97, 122, 9, 32, 39, 
	45, 65, 90, 97, 122, 65, 90, 97, 
	122, 39, 9, 32, 45, 9, 32, 45, 
	45, 62, 9, 32, 65, 90, 97, 122, 
	9, 32, 39, 123, 48, 57, 65, 90, 
	97, 122, 9, 32, 39, 123, 65, 90, 
	97, 122, 65, 90, 97, 122, 39, 9, 
	10, 32, 35, 9, 10, 32, 35, 10, 
	9, 32, 39, 125, 65, 90, 97, 122, 
	65, 90, 97, 122, 39, 9, 32, 125, 
	9, 32, 125, 9, 10, 32, 35, 39, 
	123, 48, 57, 65, 90, 97, 122, 9, 
	10, 32, 35, 39, 123, 65, 90, 97, 
	122, 9, 32, 39, 45, 62, 65, 90, 
	97, 122, 9, 32, 39, 65, 90, 97, 
	122, 9, 32, 39, 45, 48, 57, 65, 
	90, 97, 122, 10, 61, 10, 10, 101, 
	10, 110, 10, 100, 10, 9, 10, 32, 
	35, 61, 65, 90, 97, 122, 10, 61, 
	0
};

static const char _turing_single_lengths[] = {
	0, 5, 3, 1, 1, 1, 1, 1, 
	1, 1, 4, 4, 0, 1, 3, 3, 
	2, 2, 4, 4, 0, 1, 4, 4, 
	1, 4, 0, 1, 3, 3, 6, 6, 
	5, 3, 4, 2, 1, 2, 2, 2, 
	1, 5, 2
};

static const char _turing_range_lengths[] = {
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 2, 2, 0, 0, 0, 
	0, 2, 3, 2, 2, 0, 0, 0, 
	0, 2, 2, 0, 0, 0, 3, 2, 
	2, 2, 3, 0, 0, 0, 0, 0, 
	0, 2, 0
};

static const unsigned char _turing_index_offsets[] = {
	0, 0, 8, 12, 14, 16, 18, 20, 
	22, 24, 26, 34, 41, 44, 46, 50, 
	54, 57, 62, 70, 77, 80, 82, 87, 
	92, 94, 101, 104, 106, 110, 114, 124, 
	133, 141, 147, 155, 158, 160, 163, 166, 
	169, 171, 179
};

static const char _turing_indicies[] = {
	1, 2, 1, 3, 4, 5, 5, 0, 
	1, 2, 1, 0, 2, 3, 6, 0, 
	7, 0, 8, 0, 9, 0, 10, 0, 
	11, 0, 12, 12, 13, 14, 5, 15, 
	15, 0, 16, 16, 17, 18, 19, 19, 
	0, 20, 20, 0, 21, 0, 22, 22, 
	23, 0, 24, 24, 25, 0, 25, 26, 
	0, 26, 26, 27, 27, 0, 28, 28, 
	29, 31, 27, 30, 30, 0, 32, 32, 
	33, 35, 34, 34, 0, 36, 36, 0, 
	37, 0, 38, 39, 38, 40, 0, 41, 
	42, 41, 43, 0, 42, 43, 35, 35, 
	44, 41, 45, 45, 0, 46, 46, 0, 
	47, 0, 48, 48, 38, 0, 49, 49, 
	41, 0, 50, 39, 50, 40, 29, 31, 
	27, 30, 30, 0, 51, 42, 51, 43, 
	33, 35, 34, 34, 0, 52, 52, 17, 
	18, 26, 19, 19, 0, 52, 52, 17, 
	19, 19, 0, 53, 53, 13, 54, 5, 
	15, 15, 0, 56, 57, 55, 56, 55, 
	56, 58, 55, 56, 59, 55, 56, 60, 
	55, 61, 55, 1, 2, 1, 3, 4, 
	5, 5, 0, 56, 57, 55, 0
};

static const char _turing_trans_targs_wi[] = {
	0, 2, 41, 3, 4, 10, 5, 6, 
	7, 8, 9, 41, 11, 12, 32, 34, 
	11, 12, 32, 14, 13, 14, 15, 16, 
	15, 16, 17, 18, 19, 20, 30, 25, 
	19, 20, 22, 25, 21, 22, 23, 41, 
	24, 23, 41, 24, 26, 28, 27, 28, 
	29, 29, 31, 31, 33, 11, 32, 36, 
	42, 37, 38, 39, 40, 42
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
	0, 0, 0, 0, 0, 0, 9, 9, 
	9, 0, 0, 0, 0, 0, 9, 9, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int turing_start = 1;
static const int turing_first_final = 41;

static const int turing_en_doc_comment = 35;
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
    
    
    if (mSend = mSendTable[mState][mToken])
      ; // ok custom value
    else
      mSend = mSendTable[mState][0]; // use default

    if (state = mGotoTable[mState][mToken])
      mState = state;
    else
      mState = mGotoTable[mState][0]; // use default

    /* Send the value out. */
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
    int send;
    int source_state;
    int target_state;
    
    
    std::vector< std::vector<int> >::iterator it,end; // to add new tokens
    
    // init token table
    memset(mTokenTable, 0, sizeof(mTokenTable));
    
    mGotoTable.clear();
    mSendTable.clear();
    
    
#line 222 "objects/Turing.cpp"
	{
	cs = turing_start;
	}
#line 86 "objects/Turing.rl"
    
  
#line 229 "objects/Turing.cpp"
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
#line 88 "objects/Turing.rl"
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
#line 101 "objects/Turing.rl"
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
#line 120 "objects/Turing.rl"
	{ source_state = state_id; state_id = 0; }
	break;
	case 3:
#line 122 "objects/Turing.rl"
	{ target_state = state_id; state_id = 0; }
	break;
	case 4:
#line 124 "objects/Turing.rl"
	{ tok = (*p); }
	break;
	case 5:
#line 126 "objects/Turing.rl"
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
#line 154 "objects/Turing.rl"
	{
      // write transition
      #ifdef DEBUG_PARSER
      printf("SEND[%c]\n",tok);
      #endif
      send = tok;
    }
	break;
	case 7:
#line 162 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      printf("ADD %i %i %i %c\n", source_state, token_id, target_state, send);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = 0;
      source_state = 0;
      target_state = 0;
    }
	break;
	case 8:
#line 177 "objects/Turing.rl"
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
#line 191 "objects/Turing.rl"
	{ {cs = 35; goto _again;} }
	break;
	case 10:
#line 193 "objects/Turing.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 426 "objects/Turing.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}

#line 438 "objects/Turing.cpp"
	{
	const char *_acts = _turing_actions + _turing_eof_actions[cs];
	unsigned int _nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 7:
#line 162 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      printf("ADD %i %i %i %c\n", source_state, token_id, target_state, send);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = 0;
      source_state = 0;
      target_state = 0;
    }
	break;
#line 460 "objects/Turing.cpp"
		}
	}
	}

#line 216 "objects/Turing.rl"

    
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
