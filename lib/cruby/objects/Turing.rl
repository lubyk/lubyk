#include "script.h"
#define MAX_NAME_SIZE 200

%%{
  machine turing;
  write data noerror;
}%%

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

    action set_state { 
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
    
    action set_source { source_state = state_id; state_id = 0; }
    
    action set_target { target_state = state_id; state_id = 0; }

    action set_tok { tok = fc; }
    
    action set_token { 
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
    
    action set_send {
      // write transition
      #ifdef DEBUG_PARSER
      printf("SEND[%c]\n",tok);
      #endif
      send = tok;
    }
    
    action add_entry {
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
    
    action end_comment { fgoto main; }
    
    doc_comment := (('=end' %end_comment | [^\n]*) '\n')+;
    
    ws     = (' ' | '\t');
    
    identifier = (alpha alnum*) $a;
    
    tok    = ('\'' alpha @set_tok '\'' | alpha @set_tok ); # fixme: we should use 'whatever' or a-zA-Z or number

    transition = ( '-'+ '>' | '-'* ws* tok %set_token ws* '-'+ '>');
    
    send   = ('{' ws* (tok %set_send)? ws* '}' | tok %set_send );

    comment = '#' [^\n]* ;
    
    begin_comment = '=begin' %begin_comment;
    
    entry  = identifier %set_state %set_source ws* transition ws* identifier %set_state %set_target ws* send ws* comment?;

    main  := ( ws* (entry %add_entry %eof(add_entry) | comment | begin_comment | ws* )  '\n' )+ $err(error);
    write exec;
    write eof;
  }%%
    
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
