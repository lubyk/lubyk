#line 1 "objects/Turing.rl"
#include "lua_script.h"

#define MAX_NAME_SIZE 200
//#define DEBUG_PARSER

struct TuringSend
{
  TuringSend() {}
  TuringSend(int pValue) : mValue(pValue), mLuaMethod(0) {}
  TuringSend(std::string pMethod) : mMethod(pMethod), mLuaMethod(0) {}
  
  int mValue;          /**< Send a direct integer value. */
  std::string mMethod; /**< Lua method definition. */
  int mLuaMethod;      /**< Compiled lua method. */
};


#line 20 "objects/Turing.cpp"
static const char _turing_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 2, 0, 6, 2, 1, 
	4, 2, 1, 9, 2, 7, 8, 2, 
	7, 9, 3, 1, 5, 10
};

static const unsigned char _turing_key_offsets[] = {
	0, 0, 9, 10, 11, 12, 13, 14, 
	15, 17, 18, 20, 21, 22, 23, 32, 
	42, 52, 60, 66, 69, 71, 73, 79, 
	89, 99, 109, 116, 125, 135, 136, 139, 
	140, 144, 150, 153, 155, 156, 158, 160, 
	162, 163, 165, 166, 168, 170, 172, 173, 
	182, 184
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
	57, 65, 90, 97, 122, 123, 48, 57, 
	65, 90, 97, 122, 9, 32, 45, 48, 
	57, 65, 90, 97, 122, 9, 32, 40, 
	45, 48, 57, 65, 90, 97, 122, 41, 
	9, 32, 45, 125, 9, 32, 48, 57, 
	9, 10, 32, 35, 48, 57, 9, 32, 
	35, 10, 61, 10, 10, 101, 10, 110, 
	10, 100, 10, 10, 61, 10, 10, 101, 
	10, 110, 10, 100, 10, 9, 10, 32, 
	35, 61, 65, 90, 97, 122, 10, 61, 
	10, 61, 0
};

static const char _turing_single_lengths[] = {
	0, 5, 1, 1, 1, 1, 1, 1, 
	2, 1, 2, 1, 1, 1, 3, 4, 
	4, 2, 4, 3, 2, 2, 2, 4, 
	4, 4, 1, 3, 4, 1, 3, 1, 
	2, 4, 3, 2, 1, 2, 2, 2, 
	1, 2, 1, 2, 2, 2, 1, 5, 
	2, 2
};

static const char _turing_range_lengths[] = {
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 3, 3, 
	3, 3, 1, 0, 0, 0, 2, 3, 
	3, 3, 3, 3, 3, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 2, 
	0, 0
};

static const unsigned char _turing_index_offsets[] = {
	0, 0, 8, 10, 12, 14, 16, 18, 
	20, 23, 25, 28, 30, 32, 34, 41, 
	49, 57, 63, 69, 73, 76, 79, 84, 
	92, 100, 108, 113, 120, 128, 130, 134, 
	136, 140, 146, 150, 153, 155, 158, 161, 
	164, 166, 169, 171, 174, 177, 180, 182, 
	190, 193
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
	23, 23, 23, 0, 42, 40, 41, 41, 
	0, 43, 43, 44, 40, 40, 40, 0, 
	43, 43, 45, 44, 41, 41, 41, 0, 
	46, 45, 47, 47, 48, 0, 50, 49, 
	22, 22, 51, 0, 52, 53, 52, 54, 
	51, 0, 55, 55, 3, 0, 57, 58, 
	56, 57, 56, 57, 59, 56, 57, 60, 
	56, 57, 61, 56, 62, 56, 64, 65, 
	63, 64, 63, 64, 66, 63, 64, 67, 
	63, 64, 68, 63, 69, 63, 1, 2, 
	1, 3, 4, 5, 5, 0, 57, 58, 
	56, 64, 65, 63, 0
};

static const char _turing_trans_targs_wi[] = {
	0, 1, 47, 2, 3, 14, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	9, 15, 32, 15, 16, 18, 32, 25, 
	17, 21, 19, 20, 26, 19, 20, 22, 
	23, 24, 47, 2, 24, 19, 20, 26, 
	27, 28, 31, 19, 20, 29, 30, 19, 
	20, 31, 30, 33, 34, 47, 2, 34, 
	36, 48, 37, 38, 39, 40, 48, 42, 
	49, 43, 44, 45, 46, 49
};

static const char _turing_trans_actions_wi[] = {
	9, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 11, 0, 0, 0, 0, 
	15, 22, 3, 0, 0, 1, 0, 19, 
	0, 0, 31, 31, 31, 0, 0, 0, 
	1, 34, 34, 34, 0, 25, 25, 25, 
	1, 1, 0, 5, 5, 1, 1, 7, 
	7, 1, 0, 1, 28, 28, 28, 0, 
	0, 0, 0, 0, 0, 0, 13, 0, 
	0, 0, 0, 0, 0, 17
};

static const int turing_start = 1;
static const int turing_first_final = 47;

static const int turing_en_doc_comment = 35;
static const int turing_en_lua_script = 41;
static const int turing_en_main = 1;

#line 20 "objects/Turing.rl"


TuringSend gSendNothing;

class Turing : public LuaScript
{
public:
  Turing() : mTokenByName(30), mTokenNameByValue(30), mStateByName(30), mPrintBuffer(NULL), mPrintBufferSize(0)
  {
    memset(mTokenTable, 0, 256 * sizeof(int));
  }
  
  ~Turing()
  {
    clear_send_table();
    if (mPrintBuffer) free(mPrintBuffer);
  }
  
  bool set (const Params& p)
  {
    mToken = 0;
    mState = 0;
    return set_script(p);
  }

  // inlet 1
  void bang(const Signal& sig)
  { 
    int i;
    int state;
    int status;
    
    if (sig.get(&i)) {
      mRealToken = i;
      mToken = mTokenTable[ i % 256 ]; // translate token in the current machine values.
    }
    
    reload_script();
    if (!mScriptOK) return;
    
    if (mDebug) *mOutput << "{" << mState << "} -" << mRealToken << "->";
      
    if ( (mSend = mSendTable[mState][mToken]) )
      ; // ok custom value
    else
      mSend = mSendTable[mState][0]; // use default

    if ((state = mGotoTable[mState][mToken]) != -1)
      mState = state;
    else
      mState = mGotoTable[mState][0]; // use default
    
    if (mDebug) *mOutput << "{" << mState << "}" << std::endl;
    
    /* Send the value out. */
    if (mSend == &gSendNothing)
      ; // send nothing
    else if (mSend->mLuaMethod) {
      // trigger lua function
      lua_rawgeti(mLua, LUA_REGISTRYINDEX, mSend->mLuaMethod);
      /* Run the function. */
      status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
      if (status) {
        *mOutput << mName << ": trigger [" << mSend->mMethod << "] failed !\n";
        *mOutput << lua_tostring(mLua, -1) << std::endl;
      }
    } else
      send(mSend->mValue);
  }


  bool eval_script(const std::string& pScript) 
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
    TuringSend * send = &gSendNothing;
    
    // source state, target state
    std::string source;
    std::string target;
    int source_state = 0;
    int target_state = 0;
    
    // integrated lua script
    const char * begin_lua_script = NULL;
    mLuaScript = "";
    
    
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
    
    // init token table
    memset(mTokenTable, 0, sizeof(mTokenTable));
    
    mStateCount = 0;
    mTokenCount = 0;
    mGotoTable.clear();
    clear_send_table();
    mStateByName.clear();
    mStateNames.clear();
    
    
#line 276 "objects/Turing.cpp"
	{
	cs = turing_start;
	}
#line 140 "objects/Turing.rl"
    
  
#line 283 "objects/Turing.cpp"
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
#line 142 "objects/Turing.rl"
	{
      if (name_index >= MAX_NAME_SIZE) {
        *mOutput << "Name buffer overflow !\n";
        return false;
      }
      #ifdef DEBUG_PARSER
        printf("_%c_",(*p));
      #endif
      name[name_index] = (*p); /* append */
      name_index++;     
    }
	break;
	case 1:
#line 154 "objects/Turing.rl"
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
#line 163 "objects/Turing.rl"
	{
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[send " << name << "]" << std::endl;
      #endif
      send = new TuringSend((int)atoi(name));
      mSendList.push_back(send);
    }
	break;
	case 3:
#line 173 "objects/Turing.rl"
	{
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[send " << name << "]" << std::endl;
      #endif
      send = new TuringSend(std::string(name));
      mSendList.push_back(send);
    }
	break;
	case 4:
#line 183 "objects/Turing.rl"
	{
      source = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[source " << source << "]" << std::endl;
      #endif 
    }
	break;
	case 5:
#line 190 "objects/Turing.rl"
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
	case 6:
#line 200 "objects/Turing.rl"
	{ 
      if (!mTokenByName.get(&tok, std::string(name))) {
        *mOutput << "Syntax error. Unknown token '" << name << "' (missing declaration)\n";
        return false;
      }
    }
	break;
	case 7:
#line 207 "objects/Turing.rl"
	{ 
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[num " << name << "]" << std::endl;
      #endif
      tok = atoi(name);
    }
	break;
	case 8:
#line 217 "objects/Turing.rl"
	{
      mTokenByName.set(identifier, tok);
      mTokenNameByValue.set(tok, identifier);
      #ifdef DEBUG_PARSER
        std::cout << identifier << " = " << tok << std::endl;
      #endif
    }
	break;
	case 9:
#line 225 "objects/Turing.rl"
	{
      // do we know this token ?
      if (!mTokenTable[tok % 256]) {
        // new token
        #ifdef DEBUG_PARSER
        printf("new token %i: %i\n", mTokenCount, tok);
        #endif
        
        mTokenTable[tok % 256] = mTokenCount + 1;
        mTokenList.push_back(tok);
        mTokenCount++;
        
        // enlarge lookup tables (add new column)
        int counter = 0;
        for (std::vector< std::vector<int> >::iterator it = mGotoTable.begin(); it < mGotoTable.end(); it++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1)
            (*it).push_back(counter); // first value is counter (stay)
          else
            (*it).push_back(-1); // -1 means use default

          counter++;
        }
        
        for (std::vector< std::vector<TuringSend*> >::iterator it = mSendTable.begin(); it < mSendTable.end(); it++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1) {
            (*it).push_back(&gSendNothing);
          } else
            (*it).push_back(NULL); // use default 
        }
        
      }
      token_id = mTokenTable[tok % 256];
    }
	break;
	case 10:
#line 261 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      if (send->mMethod != "")
        printf("define %i - %i:%s -> %i\n", source_state, token_id, send->mMethod, target_state);
      else
        printf("define %i - %i:%i -> %i\n", source_state, token_id, send->mValue, target_state);
      #endif
      
      mGotoTable[source_state][token_id] = target_state;
      mSendTable[source_state][token_id] = send;
      token_id = 0;
      send     = &gSendNothing;
      source_state = 0;
      target_state = 0;
    }
	break;
	case 11:
#line 279 "objects/Turing.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      return false;
    }
	break;
	case 12:
#line 292 "objects/Turing.rl"
	{ {cs = 35; goto _again;} }
	break;
	case 13:
#line 293 "objects/Turing.rl"
	{ {cs = 1; goto _again;} }
	break;
	case 14:
#line 295 "objects/Turing.rl"
	{
      begin_lua_script = p;
      {cs = 41; goto _again;} 
    }
	break;
	case 15:
#line 299 "objects/Turing.rl"
	{
      mLuaScript.append( begin_lua_script, p - begin_lua_script - 4 );
      begin_lua_script = NULL;
      {cs = 1; goto _again;} 
    }
	break;
#line 547 "objects/Turing.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}

#line 559 "objects/Turing.cpp"
#line 336 "objects/Turing.rl"

  
    if (begin_lua_script) {
      mLuaScript.append( begin_lua_script, p - begin_lua_script );
    }
    // 1. for each mSendList with mMethod
    int met_count = 0;
    for(std::vector< TuringSend* >::iterator it = mSendList.begin(); it != mSendList.end(); it++) {
      if ((*it)->mMethod != "") {
        // 1.1 create function
        met_count++;
        mLuaScript.append(bprint(mPrintBuffer, mPrintBufferSize, "\nfunction trigger_%i()\n",met_count));
        mLuaScript.append((*it)->mMethod);
        mLuaScript.append("\nend\n");
      }
    }
    
    // 2. compile lua script
    if (!eval_lua_script(mLuaScript)) {
      *mOutput << mName << ": script [\n" << mLuaScript << "]\n";
      return false;
    }
    
    // 3. store lua function ids
    const char * met_function;
    met_count = 0;
    for(std::vector< TuringSend* >::iterator it = mSendList.begin(); it != mSendList.end(); it++) {
      if ((*it)->mMethod != "") {
        met_count++;
        met_function = bprint(mPrintBuffer, mPrintBufferSize, "trigger_%i",met_count);
        
        lua_getglobal(mLua, met_function); /* function to be called */

        /* take func from top of stack and store it in the Registry */
        (*it)->mLuaMethod = luaL_ref(mLua, LUA_REGISTRYINDEX);
        if ((*it)->mLuaMethod == LUA_REFNIL)
          *mOutput << mName << ": could not get lua reference for function '" << met_function << "'.\n";
        
      }
    }
    return true;
  }

  /** Output transition and action tables. */
  void tables()
  {  
    *mOutput << "tokens\n";
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value)) {
        bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %s = %i\n", i+1, identifier.c_str(), tok_value);
        *mOutput << mPrintBuffer;
        //*mOutput << " " << i << " : " << identifier << " = " << tok_value << "\n";
      } else {
        *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %i\n", i+1, tok_value);
        //*mOutput << " " << i << " : " << tok_value << "\n";
      }
    }
    print_table(*mOutput, "goto", mGotoTable);
    print_table(*mOutput, "send", mSendTable);
    
    
    *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "\n%- 7s", "methods\n");
    int met_count = 0;
    for(std::vector< TuringSend* >::iterator it = mSendList.begin(); it != mSendList.end(); it++) {
      if ((*it)->mLuaMethod) {
        met_count++;
        *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %s\n", met_count, (*it)->mMethod.c_str());
      }
    }
  }
  
  /** Output tables in digraph format to produce graphs with graphviz. */
  void dot()
  {
    make_dot_graph(*mOutput);
  }

  virtual void spy()
  { bprint(mSpy, mSpySize,"%i, %i", mTokenCount, mStateCount );  }

private:
  void clear_send_table()
  {
    std::vector< TuringSend* >::iterator it,end;
    
    end   = mSendList.end();
    for (it = mSendList.begin(); it < end; it++) {
      if ((*it)->mLuaMethod) {
        luaL_unref(mLua, LUA_REGISTRYINDEX, (*it)->mLuaMethod);
      }
      delete *it;
    }
    mSendList.clear();
    mSendTable.clear();
  }
  
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
      
      mSendTable.push_back( std::vector<TuringSend*>(mTokenCount+1, (TuringSend*)NULL) ); // NULL means use default
      mSendTable[mStateCount][0] = &gSendNothing;
      
      mStateCount++;
    }
    return state_id;
  }

  // FIXME: why is this not working ? template<typename T>
  void print_table(std::ostream& pOutput, const char * pTitle, std::vector< std::vector<int> >& pTable)
  {  
    std::vector< std::vector<int> >::iterator it, end;
    end = pTable.end();
    
    // print tokens
    pOutput << bprint(mPrintBuffer, mPrintBufferSize, "\n%- 7s -", pTitle);
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value))
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3s", identifier.c_str());
      else
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3i", tok_value);
    }
    pOutput << "\n";
    
    int state_count = 0;
    for (it = pTable.begin(); it < end; it++) {
      std::vector<int>::iterator it2,end2;
      end2 = (*it).end();
      
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3s:", mStateNames[state_count].c_str());
      for ( it2 = (*it).begin(); it2 < end2; it2++ ) {
        print_table_value(pOutput, *it2);
      }
      pOutput << "\n";
      state_count++;
    } 
  }
  
  //template<typename T>
  void print_table(std::ostream& pOutput, const char * pTitle, std::vector< std::vector<TuringSend*> >& pTable)
  {  
    std::vector< std::vector<TuringSend*> >::iterator it, end;
    end = pTable.end();
    
    // print tokens
    pOutput << bprint(mPrintBuffer, mPrintBufferSize, "\n%- 7s -", pTitle);
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value))
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3s", identifier.c_str());
      else
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3i", tok_value);
    }
    pOutput << "\n";
    
    int state_count = 0;
    for (it = pTable.begin(); it < end; it++) {
      std::vector<TuringSend*>::iterator it2,end2;
      end2 = (*it).end();
      
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3s:", mStateNames[state_count].c_str());
      for ( it2 = (*it).begin(); it2 < end2; it2++ ) {
        print_table_value(pOutput, *it2);
      }
      pOutput << "\n";
      state_count++;
    } 
  }
  
  
  void print_table_value(std::ostream& pOutput, TuringSend * pVal)
  {
    if (pVal == NULL)
      pOutput << "   -";  // default
    else if (pVal == &gSendNothing)
      pOutput << "   /";  // do not send
    else if (pVal->mLuaMethod) {
      // find method id
      std::vector<TuringSend*>::iterator it = mSendList.begin();
      int met_count = 0;
      for ( ; it != mSendList.end(); it++) {
        if ((*it)->mLuaMethod) met_count++;
        if (*it == pVal) break;
      }
      
      if (it == mSendList.end()) {
        std::cout << "turing send not in mSendList ! (" << pVal->mMethod << ", " << pVal->mValue << ").\n";
        return;
      }
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " (%i)", met_count);
    } else
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3i", pVal->mValue);
  }
  
  void print_table_value(std::ostream& pOutput, int pVal)
  {
    if (pVal == -1)
      pOutput << "   -";  // default
    else if (pVal == -2)
      pOutput << "   /";  // do not send
    else
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 3i", pVal);
  }
  
  void make_dot_graph(std::ostream& out)
  {
    std::string source,target,token,send;
    TuringSend * tsend;
    out << "digraph " << mName << "{\n";
    out << "  rankdir=LR;\n";
  	// first node
    out << "  node [ fixedsize = true, height = 0.65, shape = doublecircle ];\n";
    out << "  " << mStateNames[0] << ";\n";
    // all other nodes
    out << "  node [ shape = circle ];\n";
    // transitions
    
    for (int i=0; i < mStateCount; i++) {
      source = mStateNames[i];
      // print default action
      token = '-';
      target = mStateNames[mGotoTable[i][0]];
      tsend = mSendTable[i][0];
      if (tsend == &gSendNothing) 
        send = ""; // send nothing
      else {
        send = ":";
        if (tsend->mLuaMethod)
          send.append(tsend->mMethod);
        else
          send.append(bprint(mPrintBuffer, mPrintBufferSize, "%i", mSendTable[i][0]));
      }
      out << "  " << source << " -> " << target << " [ label = \"" << token << send << "\"];\n";
      
      // print other transitions
      for (int j=0; j < mTokenCount; j++) {
        if (!mTokenNameByValue.get(&token, mTokenList[j])) {
          bprint(mPrintBuffer, mPrintBufferSize, "%i", mTokenList[j]); // no token name
          token = mPrintBuffer;
        }
        if (mGotoTable[i][j+1] == -1)
          ;  // default, do not print
        else {
          target  = mStateNames[mGotoTable[i][j+1]];
          // source -> target [ label = "token:send" ];
          tsend = mSendTable[i][j+1];
          if (tsend == &gSendNothing) 
            send = ""; // send nothing
          else {
            send = ":";
            if (tsend->mLuaMethod)
              send.append(tsend->mMethod);
            else
              send.append(bprint(mPrintBuffer, mPrintBufferSize, "%i", mSendTable[i][0]));
          }
          out << "  " << source << " -> " << target << " [ label = \"" << token << send << "\"];\n";
        }
      }
    }
    out << "}\n";
  }
  
  int  mToken;           /**< Current token value (translated). */
  int  mRealToken;       /**< Current token value (not translated). */
  TuringSend * mSend;    /**< Send result. */
  int  mState;           /**< Current state. */
  
  int  mTokenTable[256]; /**< Translate token values into their internal representation. */
  int  mStateCount;      /**< Number of states in the machine. */
  int  mTokenCount;      /**< Number of tokens recognized by the machine. */
  
  std::string mLuaScript; /**< Lua script for method calls. */
  
  Hash<std::string, int>   mTokenByName;   /**< Dictionary returning token id from its identifier (used to  plot/debug). */
  Hash<uint, std::string>  mTokenNameByValue; /**< Dictionary returning token name from its value (used to plot/debug). */
  std::vector<int>         mTokenList;     /**< List of token values (used to plot/debug). */
  
  Hash<std::string, int>   mStateByName;   /**< Dictionary returning state id from its identifier. */
  std::vector<std::string> mStateNames;    /**< List of state names (used to plot/debug). */
  
  std::vector< std::vector<int> > mGotoTable; /**< State transition table. */
  std::vector< std::vector<TuringSend*> > mSendTable; /**< State transition table. */
  std::vector< TuringSend*> mSendList;     /**< List of sending methods. */
  
  char * mPrintBuffer;   /**< Commodity buffer to format information for printing. */
  size_t mPrintBufferSize; /**< Size of print buffer. */
  
};

extern "C" void init()
{
  CLASS (Turing)
  OUTLET(Turing, output)
  METHOD(Turing, tables)
  METHOD(Turing, dot)
  SUPER_METHOD(Turing, Script, load)
  SUPER_METHOD(Turing, Script, script)
}
