# Version
VERSION = 0.0.1

# Compiler flags
CFLAGS = -Wall -W -O2 -Wl,-x -pipe
CLMFLAGS = -Wall -W -O2 -Wl,-x -pipe -lm
LFLAGS =  -O2 -Wl,-x -pipe

all: test

test: parser.c lexer.o
	g++ -o test -O2 -Wl,-x -pipe  parser.c lexer.o -lm

lexer.o: lexer.l
	flex lexer.l
	test -e lex.yy.c && mv lex.yy.c lexer.c
	gcc -o lexer.o -c lexer.c 

parser.c: parser.y main.c lemon
	./lemon parser.y
	cat main.c >> parser.c


lemon: lemon.c
	gcc -o $@ $(LFLAGS)  $<

clean: 
	rm -f test *.out *.o parser.c parser.h lexer.c lemon