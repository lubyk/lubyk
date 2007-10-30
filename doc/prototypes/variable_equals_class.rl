#include <iostream>
#include <cstdio>

%%{
  machine foo;
  write data noerror;
}%%


void parse(char * str)
{
  char *p = str; // data pointer
  char *pe = str + strlen(str); // past end
  int cs;        // machine state
  int len = 0;
  char token[1024];
  
  %%{
    action a {
      token[len] = fc; /* append */
      len++;
    }
    
    action var {
      token[len] = '\0';
      std::cout << "[var   :" << token << "]" << std::endl;
      len = 0;
    }
    
    action key {
      token[len] = '\0';
      std::cout << "[key   :" << token << "]" << std::endl;
      len = 0;
    }
    
    action klass {
      token[len] = '\0';
      std::cout << "[klass :" << token << "]" << std::endl;;
      len = 0;
    }
    
    action space {
      printf(" ");
    }
    
    action ret {
      printf("\n");
    }
    
    action vstring {
      token[len] = '\0';
      std::cout << "[string :" << token << "]" << std::endl;;
      len = 0;
    }
    
    action vfloat {
      token[len] = '\0';
      std::cout << "[float :" << token << "]" << std::endl;;
      len = 0;
    }
    
    action vinteger {
      token[len] = '\0';
      std::cout << "[int   :" << token << "]" << std::endl;;
      len = 0;
    }
    
    ws     = (' ' | '\n' | '\t')+;
    
    identifier = 'a'..'z' @a (digit | alpha | '_')* @a;
    
    var    = identifier %var;
    
    klass  = 'A'..'Z' @a (digit | alpha | '_')* @a %klass;
    
    vstring  = '"' ([^"\\] | '\n' | ( '\\' (any | '\n') ))* @a %vstring '"';
    vfloat   = ('1'..'9' @a digit* @a '.' @a digit+ @a) %vfloat;
    vinteger = ('1'..'9' @a digit* @a) %vinteger;
    
    value  = (vstring | vfloat | vinteger);
    
    key    = identifier %key;
    
    param  = (key ':' ws* value);
    
    parameters = value | (param ws*)+;
    
    main := (var ws* '=' ws* klass '(' parameters? ')' ws*)+  ;
      
    write init;
    write exec;
  }%%

  printf("\n");
}

int main()
{
  parse("a=Value() b=Super(23.3)c=This(hey:\"mosdffasl\" come:3)\n");
}