#line 1 "objects/Turing.rl"
#include "lua_script.h"
#include <sstream>

#define MAX_NAME_SIZE 200
#define TUR_MAX_TOKEN_COUNT 256
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


#line 22 "objects/Turing.cpp"
static const char _turing_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 2, 0, 6, 2, 1, 
	4, 2, 1, 9, 2, 7, 8, 2, 
	7, 9, 3, 1, 5, 10
};

static const unsigned char _turing_key_offsets[] = {
	0, 0, 10, 11, 15, 27, 39, 50, 
	52, 59, 64, 76, 80, 83, 90, 99, 
	109, 110, 113, 114, 120, 130, 141, 145, 
	151, 154, 155, 156, 157, 158, 159, 161, 
	162, 164, 165, 166, 167, 177, 179, 180, 
	182, 184, 186, 187, 189, 190, 192, 194, 
	196, 197, 207, 209
};

static const char _turing_trans_keys[] = {
	9, 10, 32, 35, 46, 61, 65, 90, 
	97, 122, 10, 9, 32, 45, 61, 9, 
	32, 45, 46, 58, 61, 48, 57, 65, 
	90, 97, 122, 9, 32, 45, 46, 58, 
	62, 48, 57, 65, 90, 97, 122, 9, 
	32, 45, 46, 58, 48, 57, 65, 90, 
	97, 122, 45, 62, 9, 32, 46, 65, 
	90, 97, 122, 9, 10, 32, 35, 45, 
	9, 32, 35, 45, 46, 58, 48, 57, 
	65, 90, 97, 122, 9, 32, 45, 58, 
	9, 32, 45, 123, 48, 57, 65, 90, 
	97, 122, 9, 32, 45, 48, 57, 65, 
	90, 97, 122, 9, 32, 40, 45, 48, 
	57, 65, 90, 97, 122, 41, 9, 32, 
	45, 125, 9, 32, 45, 58, 48, 57, 
	9, 32, 45, 58, 48, 57, 65, 90, 
	97, 122, 9, 10, 32, 35, 45, 48, 
	57, 65, 90, 97, 122, 9, 32, 48, 
	57, 9, 10, 32, 35, 48, 57, 9, 
	32, 35, 98, 101, 103, 105, 110, 10, 
	32, 10, 32, 108, 117, 97, 10, 9, 
	32, 45, 61, 48, 57, 65, 90, 97, 
	122, 10, 61, 10, 10, 101, 10, 110, 
	10, 100, 10, 10, 61, 10, 10, 101, 
	10, 110, 10, 100, 10, 9, 10, 32, 
	35, 46, 61, 65, 90, 97, 122, 10, 
	61, 10, 61, 0
};

static const char _turing_single_lengths[] = {
	0, 6, 1, 4, 6, 6, 5, 2, 
	3, 5, 6, 4, 3, 1, 3, 4, 
	1, 3, 1, 4, 4, 5, 2, 4, 
	3, 1, 1, 1, 1, 1, 2, 1, 
	2, 1, 1, 1, 4, 2, 1, 2, 
	2, 2, 1, 2, 1, 2, 2, 2, 
	1, 6, 2, 2
};

static const char _turing_range_lengths[] = {
	0, 2, 0, 0, 3, 3, 3, 0, 
	2, 0, 3, 0, 0, 3, 3, 3, 
	0, 0, 0, 1, 3, 3, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 2, 0, 0
};

static const unsigned char _turing_index_offsets[] = {
	0, 0, 9, 11, 16, 26, 36, 45, 
	48, 54, 60, 70, 75, 79, 84, 91, 
	99, 101, 105, 107, 113, 121, 130, 134, 
	140, 144, 146, 148, 150, 152, 154, 157, 
	159, 162, 164, 166, 168, 176, 179, 181, 
	184, 187, 190, 192, 195, 197, 200, 203, 
	206, 208, 217, 220
};

static const char _turing_indicies[] = {
	1, 2, 1, 3, 4, 5, 6, 6, 
	0, 2, 3, 7, 7, 8, 9, 0, 
	10, 10, 11, 12, 14, 15, 13, 16, 
	16, 0, 17, 17, 18, 12, 14, 19, 
	13, 16, 16, 0, 17, 17, 18, 12, 
	14, 13, 16, 16, 0, 18, 19, 0, 
	19, 19, 20, 21, 21, 0, 22, 23, 
	22, 24, 25, 0, 26, 26, 3, 11, 
	12, 14, 13, 16, 16, 0, 27, 27, 
	28, 29, 0, 30, 30, 18, 0, 33, 
	31, 32, 32, 0, 34, 34, 35, 31, 
	31, 31, 0, 34, 34, 36, 35, 32, 
	32, 32, 0, 37, 36, 38, 38, 39, 
	0, 41, 40, 42, 42, 43, 44, 13, 
	0, 27, 27, 28, 29, 16, 16, 16, 
	0, 22, 23, 22, 24, 25, 21, 21, 
	21, 0, 15, 15, 45, 0, 46, 47, 
	46, 48, 45, 0, 49, 49, 3, 0, 
	50, 0, 51, 0, 52, 0, 53, 0, 
	54, 0, 55, 56, 0, 2, 0, 56, 
	57, 0, 58, 0, 59, 0, 60, 0, 
	7, 7, 8, 9, 6, 6, 6, 0, 
	62, 63, 61, 62, 61, 62, 64, 61, 
	62, 65, 61, 62, 66, 61, 67, 61, 
	69, 70, 68, 69, 68, 69, 71, 68, 
	69, 72, 68, 69, 73, 68, 74, 68, 
	1, 2, 1, 3, 4, 5, 6, 6, 
	0, 62, 63, 61, 69, 70, 68, 0
};

static const char _turing_trans_targs_wi[] = {
	0, 1, 49, 2, 3, 25, 36, 4, 
	6, 22, 4, 5, 11, 19, 13, 22, 
	20, 6, 7, 8, 9, 21, 10, 49, 
	2, 6, 10, 12, 7, 13, 12, 14, 
	15, 18, 12, 7, 16, 17, 12, 7, 
	18, 17, 12, 7, 13, 23, 24, 49, 
	2, 24, 26, 27, 28, 29, 30, 31, 
	32, 33, 34, 35, 31, 38, 50, 39, 
	40, 41, 42, 50, 44, 51, 45, 46, 
	47, 48, 51
};

static const char _turing_trans_actions_wi[] = {
	9, 0, 0, 0, 1, 0, 1, 22, 
	22, 3, 0, 0, 19, 1, 0, 0, 
	19, 0, 0, 0, 1, 1, 34, 34, 
	34, 34, 0, 25, 25, 25, 0, 1, 
	1, 0, 5, 5, 1, 1, 7, 7, 
	1, 0, 31, 31, 31, 1, 28, 28, 
	28, 0, 0, 0, 0, 0, 0, 11, 
	0, 0, 0, 0, 15, 0, 0, 0, 
	0, 0, 0, 13, 0, 0, 0, 0, 
	0, 0, 17
};

static const int turing_start = 1;
static const int turing_first_final = 49;

static const int turing_en_doc_comment = 37;
static const int turing_en_lua_script = 43;
static const int turing_en_main = 1;

#line 22 "objects/Turing.rl"


TuringSend gSendNothing;

class Turing : public LuaScript
{
public:
  Turing() : mTokenByName(30), mTokenNameByValue(30), mStateByName(30), mPrintBuffer(NULL), mPrintBufferSize(0) 
  {
    clear_tables();
  }
  ~Turing()
  {
    clear_tables();
    if (mPrintBuffer) free(mPrintBuffer);
  }
  
  bool set (const Params& p)
  {
    return set_script(p);
  }

  // inlet 1
  void bang(const Signal& sig)
  { 
    if (!mIsOK) return;
    
    int i;
    int state;
    int status;
    
    if (sig.get(&i)) {
      mRealToken = i;
      mToken = mTokenTable[ mRealToken % TUR_MAX_TOKEN_COUNT ]; // translate token in the current machine values.
    }
    
    reload_script();
    if (!mScriptOK) return;
    
    set_lua_global("in1", sig);
    call_lua(&mS, "bang");
    if (!mS.type) return; // bang returned nil, abort
    else if (mS.type != BangSignal) {
      if (mS.get(&mRealToken)) // use token from returned value
        mToken = mTokenTable[ mRealToken % TUR_MAX_TOKEN_COUNT ]; // translate token in the current machine values.
    }
    
    if (mDebug) *mOutput << "{" << mState << "} -" << mRealToken << "->";
    
    if ( !(mSend = mSendTable[mState][mToken]) ) {
      if ( (mSend = mSendTable[0][mToken]) )
        ; // ok use token default send
      else
        mSend = mSendTable[mState][0]; // use state default
    } else
      ; // ok custom value

    if ((state = mGotoTable[mState][mToken]) == -1) {
      if (mGotoTable[0][mToken] == -1)
        mState = mGotoTable[mState][0]; // use default state action
      else
        mState = mGotoTable[0][mToken]; // use default token action
    } else
      mState = state;
    
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
  
  void in2(const Signal& sig)
  { set_lua_global("in2", sig); }
  
  void in3(const Signal& sig)
  { set_lua_global("in3", sig); }
  
  void in4(const Signal& sig)
  { set_lua_global("in4", sig); }
  
  void in5(const Signal& sig)
  { set_lua_global("in5", sig);}
  
  void in6(const Signal& sig)
  { set_lua_global("in6", sig);}
  
  void in7(const Signal& sig)
  { set_lua_global("in7", sig);}
  
  void in8(const Signal& sig)
  { set_lua_global("in8", sig);}
  
  void in9(const Signal& sig)
  { set_lua_global("in9", sig);}
  
  void in10(const Signal& sig)
  { set_lua_global("in10", sig);}

  bool eval_script(const std::string& pScript) 
  {
    mToken = 0;
    mState = 1;
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
    mLuaScript = "function bang() return in1 end\n\n";
    
    
    // function call id, params
    // 1. during parse
    // 1.1 'send' ? store 0
    // 1.2 store i++ and push_back vector of method names (method_names)
    // 1.3 store arguments as vector
    // 2. during resolution
    // 2.1 foreach i, get method_names[i], push back in method_ids
    // 2.2 foreach 'send' in table, replace id by method_ids[id] (keep arguments)
    
    // a call = push args on stack, call method_id
    
    std::string identifier;
    
    // init token table
    clear_tables();
    
    
#line 320 "objects/Turing.cpp"
	{
	cs = turing_start;
	}
#line 176 "objects/Turing.rl"
    
  
#line 327 "objects/Turing.cpp"
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
#line 178 "objects/Turing.rl"
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
#line 190 "objects/Turing.rl"
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
#line 199 "objects/Turing.rl"
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
#line 209 "objects/Turing.rl"
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
#line 219 "objects/Turing.rl"
	{
      source = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[source " << source << "]" << std::endl;
      #endif
    }
	break;
	case 5:
#line 226 "objects/Turing.rl"
	{ 
      target = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[target " << target << "]" << std::endl;
      #endif
      if (source != ".")
        source_state = get_state_id(source); // we postponed this to here to be sure state is not confused with token identifier
      target_state = get_state_id(target);
      source = target; // the last target becomes the next source
    }
	break;
	case 6:
#line 237 "objects/Turing.rl"
	{ 
      if (!mTokenByName.get(&tok, std::string(name))) {
        *mOutput << "Syntax error. Unknown token '" << name << "' (missing declaration)\n";
        return false;
      }
    }
	break;
	case 7:
#line 244 "objects/Turing.rl"
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
#line 254 "objects/Turing.rl"
	{
      mTokenByName.set(identifier, tok);
      mTokenNameByValue.set(tok, identifier);
      #ifdef DEBUG_PARSER
        std::cout << identifier << " = " << tok << std::endl;
      #endif
    }
	break;
	case 9:
#line 262 "objects/Turing.rl"
	{
      // do we know this token ?
      if (!mTokenTable[tok % TUR_MAX_TOKEN_COUNT]) {
        // new token
        #ifdef DEBUG_PARSER
        printf("new token %i: %i\n", (int)mTokenCount, tok);
        #endif
        
        mTokenTable[tok % TUR_MAX_TOKEN_COUNT] = mTokenCount;
        mTokenList.push_back(tok);
        
        // enlarge lookup tables (add new column)
        int counter = 0;
        for (size_t i = 0; i < mStateCount; i++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1 && i != 0)
            mGotoTable[i].push_back(counter); // first value is counter (stay)
          else
            mGotoTable[i].push_back(-1); // -1 means use default

          counter++;
        }
        
        for (size_t i = 0; i < mStateCount; i++) {
          // enlarge all arrays in the table
          if (mTokenCount == 1 && i != 0) {
            mSendTable[i].push_back(&gSendNothing);
          } else
            mSendTable[i].push_back(NULL); // use default 
        }
        
        mTokenCount++;
      }
      token_id = mTokenTable[tok % TUR_MAX_TOKEN_COUNT];
    }
	break;
	case 10:
#line 298 "objects/Turing.rl"
	{
      // write the entry
      #ifdef DEBUG_PARSER
      if (send->mMethod != "")
        printf("define %i - %i:%s -> %i\n", source_state, token_id, send->mMethod.c_str(), target_state);
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
#line 316 "objects/Turing.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      return false;
    }
	break;
	case 12:
#line 329 "objects/Turing.rl"
	{ {cs = 37; goto _again;} }
	break;
	case 13:
#line 330 "objects/Turing.rl"
	{ {cs = 1; goto _again;} }
	break;
	case 14:
#line 332 "objects/Turing.rl"
	{
      begin_lua_script = p;
      {cs = 43; goto _again;} 
    }
	break;
	case 15:
#line 336 "objects/Turing.rl"
	{
      mLuaScript.append( begin_lua_script, p - begin_lua_script - 4 );
      begin_lua_script = NULL;
      {cs = 1; goto _again;} 
    }
	break;
#line 593 "objects/Turing.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_out: {}
	}

#line 605 "objects/Turing.cpp"
#line 373 "objects/Turing.rl"

  // token_default %add_token_default |
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
    mState = 1;
    return mStateCount > 1;
  }

  /** Output transition and action tables. */
  void tables()
  {  
    *mOutput << "tokens\n";
    for(size_t i=1; i < mTokenCount; i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value)) {
        bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %s = %i\n", i, identifier.c_str(), tok_value);
        *mOutput << mPrintBuffer;
        //*mOutput << " " << i << " : " << identifier << " = " << tok_value << "\n";
      } else {
        *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %i\n", i, tok_value);
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
  void dot(const Params& p)
  {
    std::string path;
    if (p.get(&path, "file", true)) {
      std::ofstream out(path.c_str(), std::ios::out | std::ios::binary);
      if (!out) {
        *mOutput << mName << ": could not write graph to file '" << path << "'.\n";
        return;
      }
      make_dot_graph(out);
      out.close();
      *mOutput << mName << ": graph content written to '" << path << "'.\n";
    } else {
      make_dot_graph(*mOutput);
    }
  }

  virtual void spy()
  { bprint(mSpy, mSpySize,"%i, %i", mTokenCount - 1, mStateCount - 1 );  }

private:
  void clear_tables()
  { 
    memset(mTokenTable, 0, TUR_MAX_TOKEN_COUNT * sizeof(int));
      
    mGotoTable.clear();
    mGotoTable.push_back( std::vector<int>(1, -1) ); // -1 means use default
    
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
    mSendTable.push_back( std::vector<TuringSend*>(1, (TuringSend*)NULL) ); // NULL means use default
    
    mStateByName.clear();
    mStateByName.set(std::string("-"), 0);
    
    mStateNames.clear();
    mStateNames.push_back("-");
    
    mTokenList.clear();
    mTokenList.push_back(0);
    
    mTokenByName.clear();
    
    mTokenNameByValue.clear();
    mTokenNameByValue.set(0, "-");
    
    mStateCount = 1; // first state = token default
    mTokenCount = 1; // first token (token '0') = default action/send
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

  void print_tokens(std::ostream& pOutput)
  {
    for(size_t i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value))
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5s", identifier.c_str());
      else
        pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5i", tok_value);
    }
    pOutput << "\n";
  }
  
  // FIXME: why is this not working ? template<typename T>
  void print_table(std::ostream& pOutput, const char * pTitle, std::vector< std::vector<int> >& pTable)
  { 
    // print tokens
    pOutput << bprint(mPrintBuffer, mPrintBufferSize, "\n%- 7s", pTitle);
    print_tokens(pOutput);
    
    for (size_t i=0; i < pTable.size(); i++) {
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5s:", mStateNames[i].c_str());
      for (size_t j=0; j < mTokenCount; j++)
        print_table_value(pOutput, pTable[i][j], pTable[0][j], i);
      pOutput << "\n";
    } 
  }
  
  //template<typename T>
  void print_table(std::ostream& pOutput, const char * pTitle, std::vector< std::vector<TuringSend*> >& pTable)
  {  
    // print tokens
    pOutput << bprint(mPrintBuffer, mPrintBufferSize, "\n%- 7s", pTitle);
    print_tokens(pOutput);

    for (size_t i=0; i < pTable.size(); i++) {
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5s:", mStateNames[i].c_str());
      for (size_t j=0; j < mTokenCount; j++)
        print_table_value(pOutput, pTable[i][j], pTable[0][j], i);
      pOutput << "\n";
    }
  }
  
  
  void print_table_value(std::ostream& pOutput, TuringSend * pVal, TuringSend * pTokenDefault, int pIndex)
  {
    if (pVal == NULL) {
      if (pIndex == 0)
        pOutput << "      ";
      else if (pTokenDefault != NULL)
        pOutput << "     |";  // default
      else
        pOutput << "     -";  // default
    } else if (pVal == &gSendNothing)
      pOutput << "     /";  // do not send
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
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " (% 3i)", met_count);
    } else
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5i", pVal->mValue);
  }
  
  void print_table_value(std::ostream& pOutput, int pVal, int pTokenDefault, int pIndex)
  {
    if (pVal == -1) {
      if (pIndex == 0)
        pOutput << "      ";
      else if (pTokenDefault != -1)
        pOutput << "     |";  // default
      else
        pOutput << "     -";  // default
    } else if (pVal == -2)
      pOutput << "     /";  // do not send
    else
      pOutput << bprint(mPrintBuffer, mPrintBufferSize, " % 5i", pVal);
  }
  
  void make_dot_graph(std::ostream& out)
  {
    std::string source,target,token,send;
    TuringSend * tsend;
    out << "digraph " << mName << "{\n";
    out << "  rankdir=LR;\n";
    out << "  node [ style = bold, fontsize = 12];\n";
  	// first node
  	if (mStateCount > 1) {
      out << "  node [ fixedsize = true, height = 0.5, shape = doublecircle ];\n";
      out << "  " << mStateNames[1] << ";\n";
  	}
  	
    out << "  node [ shape = circle ];\n";
    out << "  edge [ color = dimgrey];\n";
    
    // transitions
    for (size_t i=0; i < mStateCount; i++) { // loop into each state (row)
      source = mStateNames[i];
      
      for (size_t j=0; j < mTokenCount; j++) { // loop into each token (column).
        if (i == 0 && j == 0) continue; // no default_default
        
        if (!mTokenNameByValue.get(&token, mTokenList[j])) {
          bprint(mPrintBuffer, mPrintBufferSize, "%i", mTokenList[j]); // no token name
          token = mPrintBuffer;
        }
        if (mGotoTable[i][j] == -1)
          ;  // default, do not print
        else {
          if (i == 0) {
            // change source name for default token actions
            // set source name as 'any__1234' for token default (name not shown but must be unique)
            source = bprint(mPrintBuffer, mPrintBufferSize, "any__%i", j);
            out << "  node [ fixedsize = true, height = 0.15, shape = point ];\n";
            out << "  " << source << ";\n";
            out << "  node [ fixedsize = true, height = 0.5, shape = circle ];\n";
          }
          
          target  = mStateNames[mGotoTable[i][j]];
          // source -> target [ label = "token:send" ];
          tsend = mSendTable[i][j];
          if (tsend == &gSendNothing) 
            send = ""; // send nothing
          else {
            send = ":";
            if (tsend->mLuaMethod)
              send.append(tsend->mMethod);
            else
              send.append(bprint(mPrintBuffer, mPrintBufferSize, "%i", mSendTable[i][0]));
          }
          out << "  " << source << " -> " << target << " [ label = \"" << token << send << "\", fontsize = 12];\n";   
        }
      }
    }
    out << "}\n";
  }
  
  int  mToken;           /**< Current token value (translated). */
  int  mRealToken;       /**< Current token value (not translated). */
  TuringSend * mSend;    /**< Send result. */
  int  mState;           /**< Current state. */
  
  int     mTokenTable[TUR_MAX_TOKEN_COUNT]; /**< Translate token values into their internal representation. */
  size_t  mStateCount;      /**< Number of states in the machine. */
  size_t  mTokenCount;      /**< Number of tokens recognized by the machine. */
  
  std::string mLuaScript; /**< Lua script for method calls. */
  
  Hash<std::string, int>   mTokenByName;   /**< Dictionary returning token id from its identifier (used to  plot/debug). */
  Hash<uint, std::string>  mTokenNameByValue; /**< Dictionary returning token name from its value (used to plot/debug). */
  std::vector<int>         mTokenList;     /**< List of token values (used to plot/debug). */
  
  Hash<std::string, int>   mStateByName;   /**< Dictionary returning state id from its identifier. */
  std::vector<std::string> mStateNames;    /**< List of state names (used to plot/debug). */
  
  std::vector< std::vector<int> > mGotoTable;         /**< State transition table. */
  std::vector< std::vector<TuringSend*> > mSendTable; /**< State transition table. */
  std::vector< TuringSend*> mSendList;     /**< List of sending methods. */
  
  char * mPrintBuffer;   /**< Commodity buffer to format information for printing. */
  size_t mPrintBufferSize; /**< Size of print buffer. */
  
};

extern "C" void init()
{
  CLASS (Turing)
  INLET (Turing, in2)
  INLET (Turing, in3)
  INLET (Turing, in4)
  INLET (Turing, in5)
  INLET (Turing, in6)
  INLET (Turing, in7)
  INLET (Turing, in8)
  INLET (Turing, in9)
  INLET (Turing, in10)
  OUTLET(Turing, out)
  OUTLET(Turing, out2)
  OUTLET(Turing, out3)
  OUTLET(Turing, out4)
  OUTLET(Turing, out5)
  OUTLET(Turing, out6)
  OUTLET(Turing, out7)
  OUTLET(Turing, out8)
  OUTLET(Turing, out9)
  OUTLET(Turing, out10)
  METHOD(Turing, tables)
  METHOD(Turing, dot)
  SUPER_METHOD(Turing, Script, load)
  SUPER_METHOD(Turing, Script, script)
}
