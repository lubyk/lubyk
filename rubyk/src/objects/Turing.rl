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

%%{
  machine turing;
  write data noerror;
}%%

TuringSend gSendNothing;

class Turing : public LuaScript
{
public:
  Turing() : mTokenByName(30), mTokenNameByValue(30), mStateByName(30), mPrintBuffer(NULL), mPrintBufferSize(0) 
  {
    clear_tables();
    mState = 1;
  }
  ~Turing()
  {
    clear_tables();
    if (mPrintBuffer) free(mPrintBuffer);
  }
  
  bool set (const Value& p)
  {
    return set_script(p);
  }

  // inlet 1
  void bang(const Value& sig)
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
    
    call_lua(&mS, "bang", sig);
    if (!mS.type) return; // bang returned nil, abort
    else if (mS.type != BangValue) {
      if (mS.get(&mRealToken)) // use token from returned value
        mToken = mTokenTable[ mRealToken % TUR_MAX_TOKEN_COUNT ]; // translate token in the current machine values.
    }
    
    if (mDebug) *mOutput << mName << ": {" << mStateNames[mState] << "} -" << mRealToken << "-> ";
    
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
    
    if (mDebug) *mOutput << "{" << mStateNames[mState] << "}" << std::endl;
    
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
  
  void in2(const Value& sig)
  { set_lua_global("in2", sig); }
  
  void in3(const Value& sig)
  { set_lua_global("in3", sig); }
  
  void in4(const Value& sig)
  { set_lua_global("in4", sig); }
  
  void in5(const Value& sig)
  { set_lua_global("in5", sig);}
  
  void in6(const Value& sig)
  { set_lua_global("in6", sig);}
  
  void in7(const Value& sig)
  { set_lua_global("in7", sig);}
  
  void in8(const Value& sig)
  { set_lua_global("in8", sig);}
  
  void in9(const Value& sig)
  { set_lua_global("in9", sig);}
  
  void in10(const Value& sig)
  { set_lua_global("in10", sig);}

  bool eval_script(const std::string& pScript) 
  {
    mToken = 0;
    mScript = pScript;
    mScript.append("\n");
    int cs;
    const char * p  = mScript.data(); // data pointer
    const char * pe = p + mScript.size(); // past end
    const char *eof = NULL;  // FIXME: this should be set to 'pe' on the last pStr block...
    char name[MAX_NAME_SIZE + 1];
    int  name_index = 0;
    
    int token_id = 0;
    int tok;
    TuringSend * send = &gSendNothing;
    std::string old_state_name = "";
    if (mStateCount > 1)
      old_state_name = mStateNames[mState % mStateCount];
    
    // source state, target state
    std::string source;
    std::string target;
    int source_state = 0;
    int target_state = 0;
    
    // integrated lua script
    const char * begin_lua_script = NULL;
    mLuaScript = "function bang(sig) return sig end\n\n";
    
    
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
    
    %% write init;
    
  %%{
    action a {
      if (name_index >= MAX_NAME_SIZE) {
        *mOutput << "Name buffer overflow !\n";
        return false;
      }
      #ifdef DEBUG_PARSER
        printf("_%c_",fc);
      #endif
      name[name_index] = fc; /* append */
      name_index++;     
    }
    
    action set_identifier {
      name[name_index] = '\0';
      identifier = name;
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[identifier " << identifier << "]" << std::endl;
      #endif
    }
    
    action set_send {
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[send " << name << "]" << std::endl;
      #endif
      send = new TuringSend((int)atoi(name));
      mSendList.push_back(send);
    }
    
    action set_lua_send {
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[send " << name << "]" << std::endl;
      #endif
      send = new TuringSend(std::string(name));
      mSendList.push_back(send);
    }
    
    action set_source {
      source = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[source " << source << "]" << std::endl;
      #endif
    }
    
    action set_target { 
      target = identifier;
      #ifdef DEBUG_PARSER
        std::cout <<    "[target " << target << "]" << std::endl;
      #endif
      if (source != ".")
        source_state = get_state_id(source); // we postponed this to here to be sure state is not confused with token identifier
      if (target != ".")
        target_state = get_state_id(target);
      source = target; // the last target becomes the next source
    }

    action set_token_from_identifier { 
      if (!mTokenByName.get(&tok, std::string(name))) {
        *mOutput << "Syntax error. Unknown token '" << name << "' (missing declaration)\n";
        return false;
      }
    }
    
    action set_tok_value { 
      name[name_index] = '\0';
      name_index = 0;
      #ifdef DEBUG_PARSER
        std::cout <<    "[num " << name << "]" << std::endl;
      #endif
      tok = atoi(name);
    }
    
    
    action define_token {
      mTokenByName.set(identifier, tok);
      mTokenNameByValue.set(tok, identifier);
      #ifdef DEBUG_PARSER
        std::cout << identifier << " = " << tok << std::endl;
      #endif
    }
    
    action set_token {
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
    
    action add_entry {
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
    
    action error {
      fhold; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      return false;
    }
    
    action debug {
      printf("[%c]", fc);
      fflush(stdout);
    }
    
    action begin_comment { fgoto doc_comment; }
    action end_comment   { fgoto main; }
    
    action begin_lua     {
      begin_lua_script = p;
      fgoto lua_script; 
    }
    action end_lua       {
      mLuaScript.append( begin_lua_script, p - begin_lua_script - 4 );
      begin_lua_script = NULL;
      fgoto main; 
    }
    

    
    doc_comment := (('=end' %end_comment | [^\n]*) '\n')+;
    
    lua_script  := (('=end' %end_lua     | [^\n]*) '\n')+;
    
    ws     = (' ' | '\t');
    
    identifier = (alpha alnum* | '.') $a %set_identifier;
    
    tok    = ( identifier @set_token_from_identifier | digit+ $a %set_tok_value ); # fixme: we should use 'whatever' or a-zA-Z or number
    
    send   = alnum+ $a %set_send | (alpha alnum* '(' [^\)]* ')') $a %set_lua_send | '{' [^\}]* $a '}' %set_lua_send;

    transition = (ws* '-' | ws) ws* (tok %set_token)? (':' send)? ws* '-'+ '>';

    comment = '#' [^\n]* ;
    
    begin_comment = '=begin\n' @begin_comment;
    
    sub_entry = transition ws* identifier %set_target;
    
    entry     = identifier %set_source sub_entry;
    
    begin_lua = '=begin' ' '+ 'lua\n' @begin_lua;
    
    define_token = identifier ws* '=' ws* digit+ $a %set_tok_value;

    main  := ( ws* (entry %add_entry (sub_entry %add_entry)* ws* comment? | define_token %define_token (ws* comment)? | comment | begin_comment | begin_lua | ws* )  '\n' )+ $err(error);
    write exec;
  }%%
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
    if (old_state_name == "" || !mStateByName.get(&mState, old_state_name)) {
      // old name does not exist
      set_state(1);
    }
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
  void dot(const Value& p)
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
  { 
    const std::string state = (mState > 0 && (uint)mState < mStateNames.size()) ? mStateNames[(uint)mState] : "?";
    bprint(mSpy, mSpySize,"[%s] %ix%i", state.c_str(), mStateCount - 1, mTokenCount - 1);  
  }

  
  /** Set state from lua. */
  int jump()
  {
    std::string str;
    if (string_from_lua(&str)) {
      set_state(str);
    }
    return 0;
  }
  
  /** Set state from command line. */
  void jump(const Value& p)
  {
    std::string str;
    if (p.get(&str)) {
      set_state(str);
    } else {
      *mOutput << mName << ": could not get state name from parameters.\n";
    }
  }
private:
  
  /** Set state from state id. */
  void set_state(int pState)
  {
    mState = 1 + (pState - 1) % (mStateCount - 1);
  }
  
  /** Set state by state name. */
  bool set_state(std::string& pStateName)
  {
    int state_id;
    if (mStateByName.get(&state_id, pStateName)) {
      set_state(state_id);
      if (mDebug) *mOutput << mName << ": jump {" << pStateName << "}\n";
      return true;
    } else {
      return false;      
      *mOutput << mName << ": unknown state name '" << pStateName << "'.\n";
    }
  }
  
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
  METHOD(Turing, jump)
  METHOD_FOR_LUA(Turing, jump)
  SUPER_METHOD(Turing, Script, load)
  SUPER_METHOD(Turing, Script, script)
}
