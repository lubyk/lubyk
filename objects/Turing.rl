#include "script.h"

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

%%{
  machine turing;
  write data noerror;
}%%

TuringSend gSendNothing;

class Turing : public Script
{
public:
  Turing() : mTokenByName(30), mTokenNameByValue(30), mStateByName(30), mPrintBuffer(NULL), mPrintBufferSize(0) {}
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
    if (sig.get(&i)) {
      mRealToken = i;
      mToken = mTokenTable[ i % 256 ]; // translate token in the current machine values.
    }
    
    reload_script();
    if (mScriptDead) return;
    
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
      ; // trigger lua method
    } else
      send(mSend->mValue);
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
    TuringSend * send = &gSendNothing;
    
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
    
    // init token table
    memset(mTokenTable, 0, sizeof(mTokenTable));
    
    mStateCount = 0;
    mTokenCount = 0;
    mGotoTable.clear();
    clear_send_table();
    mStateByName.clear();
    mStateNames.clear();
    
    %% write init;
    
  %%{
    action a {
      if (name_index >= MAX_NAME_SIZE) {
        *mOutput << "Name buffer overflow !\n";
        mScriptDead = true;
        return;
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
      source_state = get_state_id(source); // we postponed this to here to be sure state is not confused with token identifier
      target_state = get_state_id(target);
      source = target; // the last target becomes the next source
    }

    action set_token_from_identifier { 
      if (!mTokenByName.get(&tok, std::string(name))) {
        *mOutput << "Syntax error. Unknown token '" << name << "' (missing declaration)\n";
        mScriptDead = true;
        return;
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
    
    action add_entry {
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
    
    
    action error {
      fhold; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      mScriptDead = true;
      return;
    }
    
    action debug {
      printf("[%c]", fc);
      fflush(stdout);
    }
    
    action begin_comment { fgoto doc_comment; }
    action end_comment   { fgoto main; }
    
    action begin_lua     { 
      std::cout << "begin_lua\n";
      begin_lua_script = p;
      fgoto lua_script; 
    }
    action end_lua       {
      lua_script.append( begin_lua_script, p - begin_lua_script - 4 );
      begin_lua_script = NULL;
      fgoto main; 
    }
    

    
    doc_comment := (('=end' %end_comment | [^\n]*) '\n')+;
    
    lua_script  := (('=end' %end_lua     | [^\n]*) '\n')+;
    
    ws     = (' ' | '\t');
    
    identifier = (alpha alnum*) $a %set_identifier;
    
    tok    = ( identifier @set_token_from_identifier | digit+ $a %set_tok_value ); # fixme: we should use 'whatever' or a-zA-Z or number
    
    send   = alnum+ $a %set_send;

    transition = ( '-'+ '>' | '-'* ws* tok %set_token (':' send)?  ws* '-'+ '>');

    comment = '#' [^\n]* ;
    
    begin_comment = '=begin\n' @begin_comment;
    
    sub_entry = ws+ transition ws+ identifier %set_target;
    
    entry     = identifier %set_source sub_entry;
    
    begin_lua = '=begin' ' '+ 'lua\n' @begin_lua;
    
    define_token = identifier ws* '=' ws* digit+ $a %set_tok_value;

    main  := ( ws* (entry %add_entry (sub_entry %add_entry)* (ws* comment)? | define_token %define_token (ws* comment)? | comment | begin_comment | begin_lua | ws* )  '\n' )+ $err(error);
    write exec;
    write eof;
  }%%
  
    if (begin_lua_script) {
      lua_script.append( begin_lua_script, p - begin_lua_script );
    }
    mScriptDead = false; // ok, we can receive and process signals (again).
  }

  /** Output transition and action tables. */
  void tables()
  {  
    *mOutput << "tokens\n";
    for(int i=0;i<mTokenCount;i++) {
      int tok_value = mTokenList[i];
      std::string identifier;
      if (mTokenNameByValue.get(&identifier, tok_value)) {
        bprint(mPrintBuffer, mPrintBufferSize, "% 4i: %s = %i\n", i, identifier.c_str(), tok_value);
        *mOutput << mPrintBuffer;
        //*mOutput << " " << i << " : " << identifier << " = " << tok_value << "\n";
      } else {
        *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "% 3i : %i\n", i, tok_value);
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
        *mOutput << bprint(mPrintBuffer, mPrintBufferSize, "% 3i : %s\n", met_count, (*it)->mMethod.c_str());
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
    for (it = mSendList.begin(); it < end; it++)
      delete *it;
    
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
