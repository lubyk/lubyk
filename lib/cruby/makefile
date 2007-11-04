CC=g++
CFLAGS=-g
DOT_CMD=/Applications/Graphviz.app/Contents/MacOS/dot
RAGEL=ragel
RAGEL_CD=rlgen-cd

test: test/runner test/runner.cpp
	./test/runner

rubyk: classes/main.cpp command.o rubyk.o node.o class.o objects
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. classes/main.cpp slot.o inlet.o outlet.o params.o node.o class.o command.o rubyk.o -o rubyk
	
test/runner.cpp: test/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp test/*_test.h

test/runner: test/runner.cpp command.o rubyk.o node.o class.o objects
	g++ -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o class.o command.o rubyk.o -o test/runner
	
	
slot.o: classes/slot.cpp
	g++ -c -Itemplates classes/slot.cpp -o slot.o

inlet.o: classes/inlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/inlet.cpp -o inlet.o
	
outlet.o: classes/outlet.cpp classes/Slot.cpp
	g++ -c -Itemplates classes/outlet.cpp -o outlet.o
	
params.o: classes/params.cpp classes/params.h
	g++ -c -Itemplates classes/params.cpp -o params.o
	
node.o: classes/node.cpp classes/node.h inlet.o outlet.o slot.o params.o
	g++ -c -Itemplates classes/node.cpp -o node.o

class.o: classes/class.cpp classes/class.h
	g++ -c -Itemplates classes/class.cpp -o class.o

classes/command.cpp: classes/command.rl
	${RAGEL} classes/command.rl | ${RAGEL_CD} -o classes/command.cpp

rubyk.o: classes/rubyk.h classes/rubyk.cpp
	g++ -c -Iclasses -Itemplates classes/rubyk.cpp -o rubyk.o

command.o: classes/command.cpp classes/command.h	
	g++ -c -Iclasses -Itemplates classes/command.cpp -o command.o

command.o.bak: classes/command.cpp classes/command.h parser/parser.c lexer.o	
	g++ -c -Iclasses -Itemplates classes/command.cpp parser/parser.c parser/lexer.o -O2 -Wl,-x -pipe -lm -o command.o

objects: lib/Test.rko lib/Add.rko lib/Value.rko lib/Counter.rko lib/Metro.rko lib/Print.rko lib/Midi.rko


lib/Test.rko: objects/Test.cpp classes/class.h
	g++ -o lib/Test.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Test.cpp
	
lib/Add.rko: objects/Add.cpp classes/class.h
	g++ -o lib/Add.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Add.cpp
	
lib/Value.rko: objects/Value.cpp classes/class.h
	g++ -o lib/Value.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Value.cpp

lib/Counter.rko: objects/Counter.cpp classes/class.h
	g++ -o lib/Counter.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Counter.cpp

lib/Metro.rko: objects/Metro.cpp classes/class.h
	g++ -o lib/Metro.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Metro.cpp

lib/Print.rko: objects/Print.cpp classes/class.h
	g++ -o lib/Print.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Print.cpp

lib/Midi.rko: objects/Midi.cpp classes/class.h
	g++ -o lib/Midi.rko -Itemplates -Iclasses -Iobjects -dynamic -bundle -undefined suppress -flat_namespace -D__MACOSX_CORE__ -L/usr/lib -lgcc -lstdc++ -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  objects/rtmidi/RtMidi.cpp objects/Midi.cpp

	
dot: classes/command.png	
	open classes/command.png

classes/command.dot: classes/command.rl
	ragel classes/command.rl | rlgen-dot -p -o classes/command.dot
	
classes/command.png: classes/command.dot
	${DOT_CMD} -Tpng classes/command.dot -o classes/command.png

clean:
	rm -rf *.o lib/*.rko rubyk classes/command.png classes/command.dot