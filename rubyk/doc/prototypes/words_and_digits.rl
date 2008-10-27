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
    action append {
      token[len] = fc;
      len++;
    }
    
    action digits {
      token[len] = '\0';
      std::cout << "[DIGITS:" << token << "]";
      len = 0;
    }
    
    action word {
      token[len] = '\0';
      std::cout << "[WORD:" << token << "]";
      len = 0;
    }
    
    action space {
      printf(" ");
    }
    
    action ret {
      printf("\n");
    }
    
    digits = digit+ @append %digits;
    
    word   = alpha+ @append %word;
    
    main :=
      ((digits | word) (' ' @space | '\n' @ret ) )+ ;
      
    write init;
    write exec;
  }%%

  printf("\n");
}

int main()
{
  parse("I am 32 years old\n");
}