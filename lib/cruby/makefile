CC=g++
CFLAGS=-g
DOT_CMD=/Applications/Graphviz.app/Contents/MacOS/dot

test: test/runner
	./test/runner; rm test/runner

rubyk: classes/main.cpp command.o rubyk.o node.o lib/value.rko lib/add.rko lib/counter.rko
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. classes/main.cpp slot.o inlet.o outlet.o params.o node.o command.o rubyk.o -o rubyk
	
test/runner.cpp: test/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp command.o rubyk.o node.o lib/value.rko lib/add.rko lib/counter.rko
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o command.o rubyk.o -o test/runner
	
	
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

classes/command.cpp: classes/command.rl
	ragel classes/command.rl | rlgen-cd -o classes/command.cpp

rubyk.o: classes/rubyk.h classes/rubyk.cpp
	g++ -c -Iclasses -Itemplates classes/rubyk.cpp -o rubyk.o

command.o: classes/command.cpp classes/command.h	
	g++ -c -Iclasses -Itemplates classes/command.cpp -o command.o

command.o.bak: classes/command.cpp classes/command.h parser/parser.c lexer.o	
	g++ -c -Iclasses -Itemplates classes/command.cpp parser/parser.c parser/lexer.o -O2 -Wl,-x -pipe -lm -o command.o



lib/add.rko: objects/add.cpp
	g++ -o lib/add.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/add.cpp
	
lib/value.rko: objects/value.cpp
	g++ -o lib/value.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/value.cpp

lib/counter.rko: objects/counter.cpp
	g++ -o lib/counter.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/counter.cpp

lib/metro.rko: objects/metro.cpp
	g++ -o lib/metro.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/metro.cpp

	
dot: classes/command.png	
	open classes/command.png

classes/command.png: classes/command.rl
	ragel classes/command.rl | rlgen-dot -p -o classes/command.dot
	${DOT_CMD} -Tpng classes/command.dot -o classes/command.png

clean:
	rm -rf *.o lib/*.rko rubyk classes/command.png classes/command.dot