CC=g++
CFLAGS=-g

test: test/runner
	./test/runner; rm test/runner
	
test/runner.cpp: test/*_test.h slot.o inlet.o outlet.o params.o node.o
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp slot.o inlet.o outlet.o
	g++ -Itest -Itemplates -Iclasses -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o -o test/runner
	
	
slot.o: classes/Slot.cpp
	g++ -c -Itemplates classes/Slot.cpp -o slot.o

inlet.o: classes/Inlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Inlet.cpp -o inlet.o
	
outlet.o: classes/Outlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/Outlet.cpp -o outlet.o
	
params.o: classes/Params.cpp classes/Params.h
	g++ -c -Itemplates classes/Params.cpp -o params.o
	
node.o: classes/Node.cpp classes/Node.h
	g++ -c -Itemplates classes/Node.cpp -o node.o