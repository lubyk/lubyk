#line 1 "objects/Turing.rl"
#include "script.h"
#define MAX_NAME_SIZE 200
#define DEBUG_PARSER


#line 8 "objects/Turing.cpp"
static const char _turing_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 2, 0, 5, 2, 1, 3, 2, 
	1, 8, 2, 6, 7, 2, 6, 8, 
	3, 1, 4, 9
};

static const unsigned char _turing_key_offsets[] = {
	0, 0, 9, 10, 11, 12, 13, 14, 
	15, 17, 18, 20, 21, 22, 23, 32, 
	42, 52, 60, 66, 69, 71, 73, 79, 
	89, 99, 109, 113, 122, 126, 129, 131, 
	132, 134, 136, 138, 139, 141, 142, 144, 
	146, 148, 149, 158, 160
};

static const char _turing_trans_keys[] = {
	9, 10, 32, 35, 61, 65, 90, 97, 
	122, 10, 98, 101, 103, 105, 110, 10, 
	32, 10, 32, 108, 117, 97, 10, 9, 
	32, 61, 48, 57, 65, 90, 97, 122, 
	9, 32, 45, 61, 48, 57, 65, 90, 
	97, 122, 9, 32, 45, 62, 48, 57, 
	65, 90, 97, 122, 9, 32, 48, 57, 
	65, 90, 97, 122, 9, 32, 45, 58, 
	48, 57, 9, 32, 45, 45, 62, 9, 
	32, 9, 32, 65, 90, 97, 122, 9, 
	10, 32, 35, 48, 57, 65, 90, 97, 
	122, 9, 32, 35, 45, 48, 57, 65, 
	90, 97, 122, 9, 32, 45, 58, 48, 
	57, 65, 90, 97, 122, 65, 90, 97, 
	122, 9, 32, 45, 48, 57, 65, 90, 
	97, 122, 9, 32, 48, 57, 10, 48, 
	57, 10, 61, 10, 10, 101, 10, 110, 
	10, 100, 10, 10, 61, 10, 10, 101, 
	10, 110, 10, 100, 10, 9, 10, 32, 
	35, 61, 65, 90, 97, 122, 10, 61, 
	10, 61, 0
};

static const char _turing_single_lengths[] = {
	0, 5, 1, 1, 1, 1, 1, 1, 
	2, 1, 2, 1, 1, 1, 3, 4, 
	4, 2, 4, 3, 2, 2, 2, 4, 
	4, 4, 0, 3, 2, 1, 2, 1, 
	2, 2, 2, 1, 2, 1, 2, 2, 
	2, 1, 5, 2, 2
};

static const char _turing_range_lengths[] = {
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 3, 3, 
	3, 3, 1, 0, 0, 0, 2, 3, 
	3, 3, 2, 3, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 2, 0, 0
};

static const unsigned char _turing_index_offsets[] = {
	0, 0, 8, 10, 12, 14, 16, 18, 
	20, 23, 25, 28, 30, 32, 34, 41, 
	49, 57, 63, 69, 73, 76, 79, 84, 
	92, 100, 108, 111, 118, 122, 125, 128, 
	130, 133, 136, 139, 141, 144, 146, 149, 
	152, 155, 157, 165, 168
};

static const char _turing_indicies[] = {
	1, 2, 1, 3, 4, 5, 5, 0, 
	2, 3, 6, 0, 7, 0, 8, 0, 
	9, 0, 10, 0, 11, 12, 0, 2, 
	0, 12, 13, 0, 14, 0, 15, 0, 
	16, 0, 17, 17, 18, 5, 5, 5, 
	0, 19, 19, 20, 22, 21, 23, 23, 
	0, 24, 24, 20, 25, 21, 23, 23, 
	0, 24, 24, 21, 23, 23, 0, 26, 
	26, 27, 28, 21, 0, 29, 29, 30, 
	0, 30, 25, 0, 31, 31, 0, 31, 
	31, 32, 32, 0, 33, 34, 33, 35, 
	32, 32, 32, 0, 36, 36, 3, 20, 
	21, 23, 23, 0, 37, 37, 38, 39, 
	23, 23, 23, 0, 40, 40, 0, 41, 
	41, 42, 40, 40, 40, 0, 22, 22, 
	43, 0, 44, 43, 0, 46, 47, 45, 
	46, 45, 46, 48, 45, 46, 49, 45, 
	46, 50, 45, 51, 45, 53, 54, 52, 
	53, 52, 53, 55, 52, 53, 56, 52, 
	53, 57, 52, 58, 52, 1, 2, 1, 
	3, 4, 5, 5, 0, 46, 47, 45, 
	53, 54, 52, 0
};

static const char _turing_trans_targs_wi[] = {
	0, 1, 42, 2, 3, 14, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	9, 15, 28, 15, 16, 18, 28, 25, 
	17, 21, 19, 20, 26, 19, 20, 22, 
	23, 24, 42, 2, 24, 19, 20, 26, 
	27, 19, 20, 29, 42, 31, 43, 32, 
	33, 34, 35, 43, 37, 44, 38, 39, 
	40, 41, 44
};

static const char _turing_trans_actions_wi[] = {
	7, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 9, 0, 0, 0, 0, 
	13, 20, 3, 0, 0, 1, 0, 17, 
	0, 0, 29, 29, 29, 0, 0, 0, 
	1, 32, 32, 32, 0, 23, 23, 23, 
	1, 5, 5, 1, 26, 0, 0, 0, 
	0, 0, 0, 11, 0, 0, 0, 0, 
	0, 0, 15
};

static const int turing_start = 1;
static const int turing_first_final = 42;

static const int turing_en_doc_comment = 30;
static const int turing_en_lua_script = 36;
static const int turing_en_main = 1;

#line 8 "objects/Turing.rl"


class Turing : public Script
{
public:
  Turing() : mStateByName(30), mTokenNameByValue(30), mTokenByName(30) {}
  
  bool init (const Params& p)
  {
    mToken = 0;
    mState = 0;
    return init_script(p);
  }

  // inlet 1
  void bang(const Signal& sig)
  { 
    int i;
    int state;
    if (sig.get(&i)) {
      mRealToken = i;
      mToken = mTokenTable[ i % 256 ]; // translate token in the current machine values.
    }
    
    reload_script();
    if (mScriptDead) return;
    
    //std::cout << "goto\n";
    //print(std::cout, mGotoTable);
    //std::cout << "send\n";
    //print(std::cout, mSendTable);
    //
    //std::cout << "token ["<< mToken << "]" << std::endl;
    //std::cout << "state ["<< mState << "]" << std::endl;
    
    if (mDebug) *mOutput << "{" << mState << "} -" << mRealToken << "->";
      
    if ((mSend = mSendTable[mState][mToken]) != -1)
      ; // ok custom value
    else
      mSend = mSendTable[mState][0]; // use default

    if ((state = mGotoTable[mState][mToken]) != -1)
      mState = state;
    else
      mState = mGotoTable[mState][0]; // use default
    
    if (mDebug) *mOutput << "{" << mState << "}" << std::endl;
    
    /* Send the value out. */
    if (mSend == -2)
      ; // send nothing
    else
      send(mSend);
  }


  void eval_script(const std::string& pScript) 
  {
    mScript = pScript;
    mScript.append("\n");
    int cs;
    const char * p  = mScript.data(); // data pointer
    const char * pe = p + mScript.size(); // past end
    char name[MAX_NAME_SIZE + 1];
    int  name_index = 0;
    
    int token_id = 0;
    int tok;
    int send = -2;
    
    // source state, target state
    std::string source;
    std::string target;
    int source_state = 0;
    int target_state = 0;
    
    // integrated lua script
    const char * begin_lua_script = NULL;
    std::string lua_script;
    
    
    // function call id, params
    // 1. during parse
    // 1.1 'send' ? store 0
    // 1.2 store i++ and push_back vector of method names (method_names)
    // 1.3 store arguments as vector
    // 2. during resolution
    // 2.1 foreach i, get method_names[i], push back in method_ids
    // 2.2 foreach 'send' in table, replace id by method_ids[id] (keep arguments)
    
    // a call = push args on stack, call method_id
    
    // get token values by identifier
    mTokenByName.clear();
    mTokenNameByValue.clear();
    
    std::string identifier;
    
    // to add new tokens
    std::vector< std::vector<int> >::iterator it,end;
    
    // init token table
    memset(mTokenTable, 0, sizeof(mTokenTable));
    
    mStateCount = 0; // first state is 0
    mTokenCount = 0;
    mGotoTable.clear();
    mSendTable.clear();
    mStateByName.clear();
    mStateNames.clear();
    
    
#line 245 "objects/Turing.cpp"
	{
	cs = turing_start;
	}
#line 121 "objects/Turing.rl"
    
  
#line 252 "objects/Turing.cpp"
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
#line 123 "objects/Turing.rl"
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
#line 136 "objects/Turing.rl"
	{
      name[name_index] = '\0';
      identifier = name;
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[identifier " << identifier << "]" << std::endl;
      #endif
    }
	break;
	case 2:
#line 145 "objects/Turing.rl"
	{
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[send " << name << "]" << std::endl;
      #endif
      send = (int)name[0]; //FIXME !
    }
	break;
	case 3:
#line 154 "objects/Turing.rl"
	{
      source = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[source " << source << "]" << std::endl;
      #endif 
    }
	break;
	case 4:
#line 161 "objects/Turing.rl"
	{ 
      target = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[target " << target << "]" << std::endl;
      #endif
      source_state = get_state_id(source); // we postponed this to here to be sure state is not confused with token identifier
      target_state = get_state_id(target);
      source = target; // the last target becomes the next source
    }
	break;
	case 5:
#line 171 "objects/Turing.rl"
	{ 
      if(name_index) {
        // identifier: resolve to value
        name[name_index] = '\0';
        name_index = 0;
        if (!mTokenByName.get(&tok, std::string(name))) {
          *mOutput << "Syntax error. Unknown token '" << name << "' (missing declaration)\n";
          mScriptDead = true;
          return;
        }
      } else {
        *mOutput << "Syntax error: no identifier set.\n";
        mScriptDead = true;
        return;
      }
    }
	break;
	case 6:
#line 188 "objects/Turing.rl"
	{ 
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[num " << name << "]" << std::endl;
      #endif
      tok = atoi(name);
    }
	break;
	case 7:
#line 198 "objects/Turing.rl"
	{
      mTokenByName.set(identifier, tok);
      mTokenNameByValue.set(tok, identifier);
      #ifdef DEBUG_PARSER
        std::cout << identifier << " = " << tok << std::endl;
      #endif
    }
	break;
	case 8:
#line 206 "objects/Turing.rl"
	{
      // do we know this token ?
      if (!mTokenTable[tok % 256]) {
        // new token
        #ifdef DEBUG_PARSER
        printf("new token %i: %i\n", tok, mTokenCount);
        #endif
        
        mTokenTable[tok % 256] = mTokenCount + 1;
        mTokenList.push_back(tok);
        mTokenCount++;
        
        // enlarge lookup tables (add new column)
        end = mGotoTable.end();
        int counter = 0;
        for (it = mGotoTable.begin(); it < end; it++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1)
            (*it).push_back(counter); // first value is counter (stay)
          else
            (*it).push_back(-1); // -1 means use default

          counter++;
        }
        
        end = mSendTable.end();
        for (it = mSendTable.begin(); it < end; it++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1)
            (*it).push_back(-2); // -2 means do not send (default send in first column)
          else
            (*it).push_back(-1); // -1 means use default 
        }
        
      }
      token_id = mTokenTable[tok % 256];
    }
	break;
	case 9:
#line 244 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      printf("define %i - %i:%i -> %i\n", source_state, token_id, send, target_state);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = -2;
      source_state = 0;
      target_state = 0;
    }
	break;
	case 10:
#line 259 "objects/Turing.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      mScriptDead = true;
      return;
    }
	break;
	case 11:
#line 273 "objects/Turing.rl"
	{ {cs = 30; goto _again;} }
	break;
	case 12:
#line 274 "objects/Turing.rl"
	{ {cs = 1; goto _again;} }
	break;
	case 13:
#line 276 "objects/Turing.rl"
	{ 
      std::cout << "begin_lua\n";
      begin_lua_script = p;
      {cs = 36; goto _again;} 
    }
	break;
	case 14:
#line 281 "objects/Turing.rl"
	{
      lua_script.append( begin_lua_script, p - begin_lua_script - 4 );
      begin_lua_script = NULL;
      {cs = 1; goto _again;} 
    }
	break;
#line 515 "objects/Turing.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}

#line 527 "objects/Turing.cpp"
#line 318 "objects/Turing.rl"

  
    if (begin_lua_script) {
      lua_script.append( begin_lua_script, p - begin_lua_script );
    }
    std::cout << "script:\n[" << lua_script << "]\n";
    mScriptDead = false; // ok, we can receive and process signals (again).
  }

  // FIXME: use bprint (char *& pBuffer, int& pBufferSize, const char *fmt, ...);
  void tables()
  {  
    *mOutput << "tokens\n";
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value)) {
        *mOutput << " " << i << " : " << identifier << " = " << tok_value << "\n";
      } else {
        *mOutput << " " << i << " : " << tok_value << "\n";
      }
    }
    print_table(*mOutput, "goto", mGotoTable);
    print_table(*mOutput, "send", mSendTable);
  }

private:
  
  int get_state_id(const std::string& pName)
  {
    int state_id;
    // do we know this name ?
    if (!mStateByName.get(&state_id, pName)) {
      // new state
      state_id = mStateCount;
      mStateByName.set(pName, state_id);
      mStateNames.push_back(pName);
      // add a new line to the lookup tables
      mGotoTable.push_back( std::vector<int>(mTokenCount+1, -1) ); // -1 means use default
      mGotoTable[mStateCount][0] = mStateCount; // default: stay
      
      mSendTable.push_back( std::vector<int>(mTokenCount+1, -1) ); // -1 means use default
      mSendTable[mStateCount][0] = -2; // -2 means send 'nil'
      
      mStateCount++;
    }
    return state_id;
  }

  void print_table(std::ostream& pOutput, const char * pTitle, std::vector< std::vector<int> >& pTable) {  
    std::vector< std::vector<int> >::iterator it,end;
    end = pTable.end();
    
    // print tokens
    bprint(mBuf, mBufSize, "\n%- 8s  -", pTitle);
    pOutput << mBuf;
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value))
        bprint(mBuf, mBufSize, " % 3s", identifier.c_str());
      else
        bprint(mBuf, mBufSize, " % 3i", tok_value);
      pOutput << mBuf;
    }
    pOutput << "\n";
    
    int state_count = 0;
    for (it = pTable.begin(); it < end; it++) {
      std::vector<int>::iterator it2,end2;
      end2 = (*it).end();
      
      bprint(mBuf, mBufSize, " % 3s : ", mStateNames[state_count].c_str());
      pOutput << mBuf;
      for ( it2 = (*it).begin(); it2 < end2; it2++ ) {
        if (*it2 == -1)
          pOutput << "   -";  // default
        else if (*it2 == -2)
          pOutput << "   /";  // do not send
        else {
          bprint(mBuf, mBufSize, " % 3i", *it2);
          pOutput << mBuf;
        }
      }
      pOutput << "\n";
      state_count++;
    } 
  }
  
  
  
  int  mToken;           /**< Current token value (translated). */
  int  mRealToken;       /**< Current token value (not translated). */
  int  mSend;            /**< Send result. */
  int  mState;           /**< Current state. */
  
  int  mTokenTable[256]; /**< Translate token values into their internal representation. */
  int  mStateCount;      /**< Number of states in the machine. */
  int  mTokenCount;      /**< Number of tokens recognized by the machine. */
  
  Hash<std::string, int>   mTokenByName;   /**< Dictionary returning token id from its identifier (used to  plot/debug). */
  Hash<uint, std::string>  mTokenNameByValue; /**< Dictionary returning token name from its value (used to plot/debug). */
  std::vector<int>         mTokenList;     /**< List of token values (used to plot/debug). */
  
  Hash<std::string, int>   mStateByName;   /**< Dictionary returning state id from its identifier. */
  std::vector<std::string> mStateNames;    /**< List of state names (used to plot/debug). */
  
  std::vector< std::vector<int> > mGotoTable; /**< State transition table. */
  std::vector< std::vector<int> > mSendTable; /**< State transition table. */
  
};

extern "C" void init()
{
  CLASS (Turing)
  OUTLET(Turing, output)
  METHOD(Turing, tables)
  SUPER_METHOD(Turing, Script, set)
  SUPER_METHOD(Turing, Script, load)
  SUPER_METHOD(Turing, Script, script)
}
