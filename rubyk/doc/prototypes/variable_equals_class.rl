#include <iostream>
#include <cstdio>
#define MAX_BUFFER_SIZE 2048

%%{
  machine foo;
  write data noerror;
}%%

class Command
{
public:
  void parse(char * str)
  {
    char *p = str; // data pointer
    char *pe = str + strlen(str); // past end
    int cs;        // machine state
    int len = 0;
    char token[MAX_BUFFER_SIZE + 1];
  
    %%{
      action a {
        if (len >= MAX_BUFFER_SIZE) {
          std::cerr << "Buffer overflow !" << std::endl;
          // stop parsing
          return;
        }
        token[len] = fc; /* append */
        len++;     
      }
    
      action set_var {
        token[len] = '\0';
        mVariable = token;
        len = 0;
      }
      
      action set_key {
        token[len] = '\0';
        mKey = token;
        len = 0;
      }
    
      action set_klass {
        token[len] = '\0';
        mClass = token;
        len = 0;
      }
    
      action space {
        printf(" ");
      }
    
      action ret {
        printf("\n");
      }
    
      action set_string {
        token[len] = '\0';
        mValue = token;
        len = 0;
      }
    
      action set_float {
        token[len] = '\0';
        mValue = token;
        len = 0;
      }
    
      action set_integer {
        token[len] = '\0';
        mValue = token;
        len = 0;
      }
      
      action set_from {
        mFromPort = atoi(mValue.c_str());
        mFrom = mVariable;
      }
    
      action create_instance {
        std::cout << "NEW  (" << mVariable << "=" << mClass << "(" << mParams << ")" << ")" << std::endl;
        mParams = "";
      }
    
      action add_param {
        if (mParams != "")
          mParams.append(" ");
        mParams.append(mKey);
        mParams.append(":");
        mParams.append(mValue);
      }
      
      action create_link {
        mToPort = atoi(mValue.c_str());
        mTo   = mVariable;
        std::cout << "LINK (" << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << ")" << std::endl;
      }
    
      ws     = (' ' | '\n' | '\t')+;
    
      identifier = 'a'..'z' @a (digit | alpha | '_')* @a;
    
      var    = identifier %set_var;
    
      klass  = 'A'..'Z' @a (digit | alpha | '_')* @a %set_klass;
    
      string  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* @a %set_string '"';
      float   = ('1'..'9' @a digit* @a '.' @a digit+ @a) %set_float;
      integer = ('1'..'9' @a digit* @a) %set_integer;
    
      value  = (string | float | integer);
    
      key    = identifier %set_key;
    
      param  = (key ':' ws* value) %add_param;
    
      parameters = value | (param ws*)+;
    
      create_instance = var ws* '=' ws* klass '(' parameters? ')' %create_instance;
    
      create_link = var '.' integer @set_from ws* '=>' ws* integer '.' var %create_link;
    
      main := ((create_instance | create_link) ws*)+  ;
      
      write init;
      write exec;
    }%%

    printf("\n");
  }
private:
  std::string mVariable, mFrom, mTo, mClass, mValue, mKey, mParams;
  int         mFromPort,     mToPort;
};

int main()
{
  Command cmd;
  cmd.parse("a=Value() b=Super(23.3)c=This(hey:\"mosdffasl\" come:3)\na.1=>1.b a.2=>2.b");
}