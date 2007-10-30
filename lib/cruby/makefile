CC=g++
CFLAGS=-g

test: test/runner
	./test/runner; rm test/runner
	
test/runner.cpp: test/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp command.o lib/value.rko lib/add.rko lib/counter.rko
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o command.o -o test/runner
	
	
slot.o: classes/Slot.cpp
	g++ -c -Itemplates classes/Slot.cpp -o slot.o

inlet.o: classes/Inlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Inlet.cpp -o inlet.o
	
outlet.o: classes/Outlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Outlet.cpp -o outlet.o
	
params.o: classes/Params.cpp classes/params.h
	g++ -c -Itemplates classes/Params.cpp -o params.o
	
node.o: classes/Node.cpp classes/node.h inlet.o outlet.o slot.o params.o
	g++ -c -Itemplates classes/Node.cpp -o node.o

command.o: classes/command.cpp classes/command.h node.o	
	g++ -c -Iclasses -Itemplates classes/command.cpp -o command.o

command.o.bak: classes/command.cpp classes/command.h parser/parser.c lexer.o	
	g++ -c -Iclasses -Itemplates classes/command.cpp parser/parser.c parser/lexer.o -O2 -Wl,-x -pipe -lm -o command.o

lexer.c: parser/lexer.l
	flex parser/lexer.l
	test -e lex.yy.c && mv lex.yy.c lexer.c

lexer.o: lexer.c
	gcc -o lexer.o -c lexer.c

parser.c: parser.y main.c lemon
	./lemon parser.y
	cat main.c >> parser.c
	
rubyk.o: classes/rubyk.cpp classes/rubyk.h
	g++ -c -Itemplates classes/rubyk.cpp -o rubyk.o

lib/add.rko: objects/Add.cpp
	g++ -o lib/add.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Add.cpp

	
lib/value.rko: objects/Add.cpp
	g++ -o lib/value.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Value.cpp

lib/counter.rko: objects/Counter.cpp
	g++ -o lib/counter.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Counter.cpp
	

clean:
	rm -rf *.o lib/*.rko