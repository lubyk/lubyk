CC=g++
CFLAGS=-g
DOT_CMD=/Applications/Graphviz.app/Contents/MacOS/dot
RAGEL=ragel
RAGEL_CD=rlgen-cd
PLAT= macosx
TEST=test/*_test.h test/objects/*_test.h

test: test/runner test/runner.cpp
	./test/runner && rm test/runner.cpp

rubyk: classes/main.cpp command.o rubyk.o node.o inlet.o outlet.o slot.o params.o class.o lua.o classes/lua/src/liblua.a objects
	$(CC) $(CFLAGS) -o rubyk -Itest -Itemplates -Iclasses -Iobjects -I. classes/main.cpp slot.o inlet.o outlet.o params.o node.o class.o command.o rubyk.o lua.o classes/lua/src/liblua.a
	
test/runner.cpp: test/*_test.h test/objects/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)
	
test/runner: test/runner.cpp command.o rubyk.o node.o inlet.o outlet.o slot.o params.o class.o lua.o classes/lua/src/liblua.a objects
	$(CC) $(CFLAGS) -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o node.o class.o command.o rubyk.o lua.o classes/lua/src/liblua.a -o test/runner

slot.o: classes/slot.cpp classes/slot.h
	$(CC) $(CFLAGS) -c -Itemplates classes/slot.cpp -o slot.o

inlet.o: classes/inlet.cpp classes/inlet.h
	$(CC) $(CFLAGS) -c -Itemplates classes/inlet.cpp -o inlet.o
	
outlet.o: classes/outlet.cpp classes/outlet.h
	$(CC) $(CFLAGS) -c -Itemplates classes/outlet.cpp -o outlet.o
	
params.o: classes/params.cpp classes/params.h
	$(CC) $(CFLAGS) -c -Itemplates classes/params.cpp -o params.o
	
node.o: classes/node.cpp classes/node.h
	$(CC) $(CFLAGS) -c -Itemplates classes/node.cpp -o node.o

class.o: classes/class.cpp classes/class.h
	$(CC) $(CFLAGS) -c -Itemplates classes/class.cpp -o class.o

classes/command.cpp: classes/command.rl
	${RAGEL} classes/command.rl | ${RAGEL_CD} -o classes/command.cpp

rubyk.o: classes/rubyk.h classes/rubyk.cpp classes/mutex.h
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates classes/rubyk.cpp -o rubyk.o

command.o: classes/command.cpp classes/command.h classes/mutex.h
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates classes/command.cpp -o command.o

command.o.bak: classes/command.cpp classes/command.h parser/parser.c lexer.o	
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates classes/command.cpp parser/parser.c parser/lexer.o -O2 -Wl,-x -pipe -lm -o command.o

objects: lib/Test.rko lib/Add.rko lib/Value.rko lib/Counter.rko lib/Metro.rko lib/Print.rko lib/Midi.rko lib/NoteOut.rko lib/Lua.rko lib/Serial.rko lib/Turing.rko lib/Keyboard.rko lib/Cabox.rko


lib/Test.rko: objects/Test.cpp
	$(CC) $(CFLAGS) -o lib/Test.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Test.cpp
	
lib/Add.rko: objects/Add.cpp
	$(CC) $(CFLAGS) -o lib/Add.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Add.cpp
	
lib/Value.rko: objects/Value.cpp
	$(CC) $(CFLAGS) -o lib/Value.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Value.cpp

lib/Counter.rko: objects/Counter.cpp
	$(CC) $(CFLAGS) -o lib/Counter.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Counter.cpp

lib/Metro.rko: objects/Metro.cpp
	$(CC) $(CFLAGS) -o lib/Metro.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Metro.cpp

lib/Print.rko: objects/Print.cpp
	$(CC) $(CFLAGS) -o lib/Print.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Print.cpp

lib/Midi.rko: objects/Midi.cpp
	$(CC) $(CFLAGS) -o lib/Midi.rko -Itemplates -Iclasses -Iobjects -dynamic -bundle -undefined suppress -flat_namespace -D__MACOSX_CORE__ -L/usr/lib -lgcc -lstdc++ -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  objects/rtmidi/RtMidi.cpp objects/Midi.cpp

lib/NoteOut.rko: objects/NoteOut.cpp
	$(CC) $(CFLAGS) -o lib/NoteOut.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/NoteOut.cpp

lib/Lua.rko: objects/Lua.cpp
	$(CC) $(CFLAGS) -o lib/Lua.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Lua.cpp
	
lib/Serial.rko: objects/Serial.cpp objects/serial/serial.h
	$(CC) $(CFLAGS) -o lib/Serial.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Serial.cpp -DCOMPILE_SERIAL_OBJECT

lib/Cabox.rko: objects/serial/serial.h objects/Cabox.cpp
	$(CC) $(CFLAGS) -o lib/Cabox.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Serial.cpp objects/Cabox.cpp

lib/Turing.rko: objects/Turing.cpp objects/Turing.rl
	$(CC) $(CFLAGS) -o lib/Turing.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Turing.cpp

lib/Keyboard.rko: objects/Keyboard.cpp
	$(CC) $(CFLAGS) -o lib/Keyboard.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Keyboard.cpp

objects/Turing.cpp: objects/Turing.rl
	${RAGEL} objects/Turing.rl | ${RAGEL_CD} -o objects/Turing.cpp

lua.o: classes/lua/lua_script.cpp classes/lua_script.h classes/script.h
	$(CC) $(CFLAGS) -c -o lua.o -Itemplates -Iclasses -Iclasses/lua/src classes/lua/lua_script.cpp

classes/lua/src/liblua.a:
	cd classes/lua/src && make $(PLAT)
	
dot: classes/command.png	
	open classes/command.png

classes/command.dot: classes/command.rl
	ragel classes/command.rl | rlgen-dot -p -o classes/command.dot
	
classes/command.png: classes/command.dot
	${DOT_CMD} -Tpng classes/command.dot -o classes/command.png

clean:
	rm -rf *.o lib/*.rko rubyk classes/command.png classes/command.dot