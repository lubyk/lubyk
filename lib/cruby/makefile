CC=g++
CFLAGS=-g

test: test/runner
	./test/runner; rm test/runner
	
test/runner.cpp: test/*_test.h templates/orderedList.h classes/Slot.o
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp slot.o inlet.o outlet.o
	g++ -Itest -Itemplates -Iclasses -I. test/runner.cpp slot.o inlet.o outlet.o -o test/runner
	
	
slot.o: classes/Slot.cpp
	g++ -c classes/Slot.cpp -o slot.o

inlet.o: classes/Inlet.cpp classes/Slot.cpp
	g++ -c classes/Inlet.cpp -o inlet.o
	
outlet.o: classes/Outlet.cpp classes/Slot.cpp
	g++ -c classes/Outlet.cpp -o outlet.o