CC=gcc
CFLAGS=-g

test/rubyk_test: test/rubyk_test.c test/cgreen/cgreen.a test/cgreen/cgreen.h memory.o time_queue.o errors.o
	gcc memory.o time_queue.o errors.o test/rubyk_test.c test/cgreen/cgreen.a -o test/rubyk_test

test: test/rubyk_test
	./test/rubyk_test; rm test/rubyk_test

test/cgreen/cgreen.a:
	cd test/cgreen; make

memory: memory.c memory.h
	gcc memory.c -o memory.o 
    
time_queue: time_queue.c time_queue.h
	gcc time_queue.c -o time_queue.o
	
errors: errors.c errors.h
	gcc errors.c -o errors.o