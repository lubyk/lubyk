INCLUDE_HEADERS=-I../rubyk/src/templates -Ivalues -Ioscpack -I..
TESTING=-D_TESTING_
AR = ar rcu
LIBTOOL=libtool -static
TEST=test/*_test.h
OBJECTS=object.o root.o #receive.o send.o root.o zeroconf.o oscpack/liboscpack.a
CFLAGS=-g -Wall $(TESTING)


test: test/runner test/runner.cpp
	./test/runner; rm test/runner
	
# TODO: archive (ar ..)
liboscit.a: $(OBJECTS)
	$(LIBTOOL) $(OBJECTS) -o $@

oscpack/liboscpack.a:
	cd oscpack && make liboscpack.a

test/runner.cpp: $(TEST)
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)
	
test/runner: test/runner.cpp liboscit.a
	$(CC) $(CFLAGS) $(LFLAGS) -Itest $(INCLUDE_HEADERS) -I. test/runner.cpp liboscit.a -lgcc -lstdc++ -o test/runner

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

clean:
	rm -rf *.o *.dSYM liboscit.a test/runner.cpp test/*.dSYM
