#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include "actionbuilder.h"

#define MAX_BUFFER_SIZE 1024

extern FILE *yyin;  // ???
typedef struct yy_buffer_state *YY_BUFFER_STATE; // ???

Token yylval; // global variable containing the tokens returned by the lexer.

extern "C" {
  // access to the lexer
  int             yylex( void );
  YY_BUFFER_STATE yy_scan_string( const char * );
  void            yy_delete_buffer( YY_BUFFER_STATE );
}

int main(int argc,char** argv)
{
  int size, token_type;
  char buf[MAX_BUFFER_SIZE+1];
  void* pParser = ParseAlloc (malloc); // Create a new lemon Parser
	ActionBuilder mBuilder;
  
  // initialize global token container
  yylval.number = 0;
  yylval.str    = '\0';
  
  // set debugging on
  
  // uncomment to debug
  ParseTrace(stdout, ">>");

  printf("Parser started.");
  
  printf("\n> ");
  fflush(stdout);
  
  while ( ( size = read(fileno(stdin), buf, MAX_BUFFER_SIZE )) >  0) {
    // read until EOF
    buf[size]='\0';
    
    // feed lexer with the buffer
    yy_scan_string(buf);
    
    while( (token_type = yylex()) != 0 && (token_type != L_QUIT)) {
      // send found tokens until end of buffer
      Parse (pParser, token_type, yylval, &mBuilder);
      fflush(stdout);
    }
    if (token_type == L_QUIT) {
	    Parse (pParser, token_type, yylval, &mBuilder);
      printf("Bye...\n");
      break;
    } else {
      printf("\n> ");
      fflush(stdout);
    }
  }
  
  // send the EOF token
  Parse (pParser, 0, yylval, &mBuilder);
  
  // cleanup
  ParseFree(pParser, free );
}
