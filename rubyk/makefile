CC=g++
TESTING=-D_TESTING_
CFLAGS=-g -Wall -DUSE_READLINE $(TESTING)
PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
BINDIR=$(PREFIX)/bin

RUBYK_LIBDIR=$(LIBDIR)/rubyk

DOT_CMD=/Applications/Graphviz.app/Contents/MacOS/dot
RAGEL=ragel
PLAT= macosx



TEST=test/*_test.h test/objects/*_test.h
LINKER_OBJECTS=../oscit/liboscit.a slot.o inlet.o outlet.o node.o worker.o event.o class.o class_finder.o group.o text_command.o planet.o lua_script.o lua_inlet.o src/lib/lua/liblua.a # buffer.o
INCLUDE_HEADERS=-Isrc/templates -Isrc/core -Isrc/objects -Isrc/lib/lua -Isrc/lib -I../oscit/include -I../oscit/oscpack -Isrc/core/values -Isrc/lib/osc
OBJECT_COMPILE=$(INCLUDE_HEADERS) $(DEFINES) -dynamic -bundle -undefined suppress -flat_namespace -L/usr/lib -lgcc -lstdc++
OBJECT_DEPENDS=src/core/node.hpp src/core/class.h

# Utilities.
INSTALL= install -C
MKDIR= mkdir -p

all: test

test: test/runner test/runner.cpp
	./test/runner && rm test/runner

rubyk: src/core/main.cpp $(LINKER_OBJECTS) test_objects
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDE_HEADERS) src/core/main.cpp $(LINKER_OBJECTS) -o rubyk

lib:
	mkdir lib
	
lib/lua: lib src/objects/lua/*.lua
	mkdir lib/lua; cp src/objects/lua/*.lua lib/lua/

test_objects: lib lib/lua lib/Value.rko lib/Print.rko lib/Metro.rko lib/MidiOut.rko lib/NoteOut.rko lib/Lua.rko

objects: lib lib/Test.rko lib/Number.rko lib/Add.rko lib/Inlet.rko lib/Group.rko lib/Alias.rko lib/Outlet.rko lib/Print.rko lib/Counter.rko lib/Metro.rko lib/lua/rubyk.lua

off: lib/Serial.rko lib/Turing.rko lib/Keyboard.rko lib/Cabox.rko lib/Svm.rko lib/Buffer.rko lib/GLWindow.rko lib/Cut.rko lib/MaxCount.rko lib/FFT.rko lib/VQ.rko lib/ClassRecorder.rko lib/PCA.rko lib/Average.rko lib/Peak.rko lib/Minus.rko lib/Replay.rko lib/Kmeans.rko lib/Abs.rko lib/Sum.rko lib/Ctrl.rko lib/Diff.rko lib/Bang.rko lib/Macro.rko lib/MidiIn.rko lib/GLLua.rko lib/GLPlot.rko lib/Group.rko lib/System.rko
	
	
test/runner: test/runner.cpp $(LINKER_OBJECTS) #objects
	$(CC) $(CFLAGS) $(LFLAGS) -Itest $(INCLUDE_HEADERS) -I. test/runner.cpp $(LINKER_OBJECTS) -o test/runner

src/core/text_command.cpp: src/core/text_command.rl
	${RAGEL} src/core/text_command.rl -o src/core/text_command.cpp

tmatrix.o: src/core/tmatrix.cpp src/templates/tmatrix.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@
	
hash_value.o: src/core/values/hash_value.cpp src/core/values/hash_value.h src/core/values/value.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@
  
value.o: src/core/values/value.cpp src/core/values/value.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@
	
%.o: src/core/%.cpp src/core/%.h
	$(CC) $(CFLAGS) -c $(INCLUDE_HEADERS) $< -o $@
	
%.cpp: %.rl
	$(RAGEL) $< -o $@
	
lib/MidiOut.rko: src/objects/MidiOut.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  src/objects/midi/RtMidi.cpp src/objects/MidiOut.cpp -o $@
	
lib/MidiIn.rko: src/objects/MidiIn.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE)  -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  src/objects/midi/RtMidi.cpp src/objects/MidiIn.cpp -o $@

lib/Serial.rko: src/objects/Serial.cpp src/objects/serial/serial.h $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -DCOMPILE_SERIAL_OBJECT src/objects/Serial.cpp -o $@

src/objects/Turing.cpp: src/objects/Turing.rl
	${RAGEL} src/objects/Turing.rl -o src/objects/Turing.cpp
	
lib/Svm.rko: src/objects/Svm.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) src/objects/Svm.cpp src/objects/svm/svm.cpp -o $@

lib/FFT.rko: src/objects/FFT.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -Isrc/objects/fft src/objects/FFT.cpp -o $@
	
lib/VQ.rko: src/objects/VQ.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -Isrc/objects/vq src/objects/VQ.cpp src/objects/vq/elbg.c src/objects/vq/random.c -o $@
	
lib/GLWindow.rko: src/objects/GLWindow.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -Isrc/objects/gllua -framework GLUT -framework OpenGL -framework Cocoa src/objects/GLWindow.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c -o $@

lib/GLLua.rko: src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -Isrc/objects/gllua -framework GLUT -framework OpenGL -framework Cocoa src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c -o $@
  
lib/GL%.rko: src/objects/GL%.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -framework GLUT -framework OpenGL -framework Cocoa $< -o $@
	
lib/%.rko: src/objects/%.cpp $(OBJECT_DEPENDS)
	$(CC) $(CFLAGS) $(OBJECT_COMPILE) $< -o $@

src/lib/lua/liblua.a:
	cd src/lib/lua && make $(PLAT)

../oscit/liboscit.a:
	cd ../oscit && make liboscit.a
	
dot: src/core/text_command.png	
	open src/core/text_command.png

src/core/command.dot: src/core/command.rl
	ragel src/core/command.rl | rlgen-dot -p -o src/core/command.dot
	
src/core/text_command.png: src/core/text_command.dot
	${DOT_CMD} -Tpng src/core/text_command.dot -o src/core/text_command.png

install: install_rubyk install_libs

install_rubyk:
	$(MKDIR) $(BINDIR)
	$(INSTALL) rubyk $(BINDIR)

install_libs:
	$(MKDIR) $(RUBYK_LIBDIR)
	cd lib && for e in *.rko ; do echo $(RUBYK_LIBDIR)/$$e && $(INSTALL) $$e $(RUBYK_LIBDIR) ; done

clean:
	rm -rf *.o *.dSYM lib/*.rko lib/*.dSYM rubyk src/core/text_command.png src/core/text_command.dot test/runner test/runner.cpp test/*.dSYM
