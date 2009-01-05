INCLUDE_HEADERS=-I../rubyk/src/templates -I../rubyk/src/core -I../rubyk/src/core/values -Ioscpack -I..
TESTING=-D_TESTING_
AR = ar rcu
LIBTOOL=libtool -static
OBJECTS=object.o receive.o send.o root.o zeroconf.o oscpack/liboscpack.a
CFLAGS=-g -Wall $(TESTING)

# TODO: archive (ar ..)
liboscit.a: $(OBJECTS)
	$(LIBTOOL) $(OBJECTS) -o $@

oscpack/liboscpack.a:
	cd oscpack && make liboscpack.a
	
%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@

clean:
	rm -rf *.o *.dSYM liboscit.a
