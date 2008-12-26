INCLUDE_HEADERS=-I../../templates -I../../core -I../../core/values -Ioscpack -I..
AR = ar rcu
LIBTOOL=libtool -static
OBJECTS=object.o receive.o send.o root.o oscpack/liboscpack.a

# TODO: archive (ar ..)
liboscit.a: $(OBJECTS)
	$(LIBTOOL) $? -o $@

oscpack/liboscpack.a:
	cd osc && make liboscpack.a
	
%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@