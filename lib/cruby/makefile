CC=g++
CFLAGS=-g

test: test/runner
	./test/runner; rm test/runner
	
test/runner.cpp: test/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp slot.o inlet.o outlet.o params.o node.o lib/value.rko lib/add.rko lib/counter.rko
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o -o test/runner
	
	
slot.o: classes/Slot.cpp
	g++ -c -Itemplates classes/Slot.cpp -o slot.o

inlet.o: classes/Inlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Inlet.cpp -o inlet.o
	
outlet.o: classes/Outlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Outlet.cpp -o outlet.o
	
params.o: classes/Params.cpp classes/params.h
	g++ -c -Itemplates classes/Params.cpp -o params.o
	
node.o: classes/Node.cpp classes/node.h
	g++ -c -Itemplates classes/Node.cpp -o node.o
	

lib/add.rko: objects/Add.cpp
	g++ -o lib/add.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Add.cpp

	
lib/value.rko: objects/Add.cpp
	g++ -o lib/value.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Value.cpp

lib/counter.rko: objects/Counter.cpp
	g++ -o lib/counter.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Counter.cpp