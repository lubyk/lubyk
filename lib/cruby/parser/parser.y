/** This file contains the grammar needed by the parser. */

%include {
#include "token.h"   // contains the definition of the Token struct
#include "parser.h"  // contains the defines for the tokens

#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>  // strdup
#include <stdlib.h>  // free
#include "actionbuilder.h"


#define MAX_BUFFER_SIZE 1024

static char buf[MAX_BUFFER_SIZE];

void token_destructor(Token& t)
{
  // free allocated memory
}

}  // %include


%token_type   {Token}
%default_type {Token}
%token_destructor { token_destructor($$); }

%extra_argument { ActionBuilder *mBuilder }

%type expr {Token}
%type id   {Token}
  
%token_prefix L_

%parse_accept {
  // this code is called when the parsing is over.
}

   
%syntax_error {  
    printf("Syntax error.\n");
}   

main        ::= commands.
commands    ::= .                             /* can be empty  */
commands    ::= commands ws command.          /* many commands */

/* variable = Clase(blah:"hehe" boo:12) */            
command     ::= variable(v) EQUAL class(c) parameters(p). { mBuilder->createInstance(v,c,p); mBuilder->print(); }

/* call method: value1.spy() */
command     ::= IDENTIFIER DOT IDENTIFIER parameters. { printf("[ call  ]\n"); }
/* call method: value1.spy   (without params)*/
command     ::= IDENTIFIER DOT IDENTIFIER. { printf("[ call  ]\n"); }

/* value2.1 => 2.sum  (link)*/
command     ::= link_from ARROW link_to.         { printf("[new link  ]\n"); }

variable    ::= IDENTIFIER ws.
class       ::= ws CONST_IDENTIFIER ws.
parameters  ::= OPEN_PAR ws CLOSE_PAR.
parameters  ::= OPEN_PAR value CLOSE_PAR.
parameters  ::= OPEN_PAR param_list CLOSE_PAR.

link_from   ::= IDENTIFIER DOT INTEGER ws.
link_to     ::= ws INTEGER DOT IDENTIFIER.


/* 123.34 */
value       ::= ws STRING ws.
/* "some text" */
value       ::= ws FLOAT ws.
value       ::= ws INTEGER ws.
/* <<-HERE_STRING */
value       ::= ws HERESTRING ws.

param_list  ::= ws param.
param_list  ::= param_list param.         /* id:345 name:"John Difool " */

param       ::= IDENTIFIER DDOTS value. /* id:345 */

/*
command     ::= NUMBER(value). { printf("(%.0f) ", value.number); }         
command     ::= WORD(value).   { printf("{%s} ", value.str); free(value.str); }
command     ::= PUNCTUATION(punct). { printf("|%s| ", punct.str); free(punct.str); }

command_sep ::= WHITE_SPACE.
command_sep ::= command_sep WHITE_SPACE.
*/
ws          ::= .
ws          ::= ws WHITESPACE.
command     ::= QUIT.