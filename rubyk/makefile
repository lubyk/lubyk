CC=g++
TESTING=-D_TESTING_
CFLAGS=-g -Wall -DUSE_READLINE $(TESTING)
LFLAGS=-framework Accelerate -lreadline
PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
BINDIR=$(PREFIX)/bin

RUBYK_LIBDIR=$(LIBDIR)/rubyk

DOT_CMD=/Applications/Graphviz.app/Contents/MacOS/dot
RAGEL=ragel
RAGEL_CD=rlgen-cd
PLAT= macosx
TEST=test/*_test.h test/objects/*_test.h
LINKER_OBJECTS=tmatrix.o value.o hash_value.o object.o slot.o inlet.o outlet.o buffer.o class.o node.o root.o new_method.o class_finder.o group.o planet.o command.o event.o src/lib/osc/liboscpack.a
OFF_OBJ=lua_script.o src/lib/lua/liblua.a 
INCLUDE_HEADERS=-Isrc/templates -Isrc/core -Isrc/objects -Isrc/lib/lua -Isrc/lib/osc/osc -Isrc/core/values -Isrc/lib/osc
OBJECT_COMPILE=$(INCLUDE_HEADERS) -D__$(PLAT)__ -dynamic -bundle -undefined suppress -flat_namespace -L/usr/lib -lgcc -lstdc++
OBJECT_DEPENDS=src/core/node.hpp src/core/class.h

# Utilities.
INSTALL= install -C
MKDIR= mkdir -p

all: test

test: test/runner test/runner.cpp
	./test/runner && rm test/runner

rubyk: src/core/main.cpp $(LINKER_OBJECTS) objects
	$(CC) $(CFLAGS) $(LFLAGS) -o rubyk $(INCLUDE_HEADERS) -I. src/core/main.cpp $(LINKER_OBJECTS)

lib:
	mkdir lib

objects: lib lib/Test.rko lib/Number.rko lib/Add.rko lib/Inlet.rko lib/Group.rko lib/Alias.rko lib/Outlet.rko lib/Print.rko lib/Counter.rko lib/Metro.rko

off: lib/MidiOut.rko lib/NoteOut.rko lib/Lua.rko lib/Serial.rko lib/Turing.rko lib/Keyboard.rko lib/Cabox.rko lib/Svm.rko lib/Buffer.rko lib/GLWindow.rko lib/Cut.rko lib/MaxCount.rko lib/FFT.rko lib/VQ.rko lib/ClassRecorder.rko lib/PCA.rko lib/Average.rko lib/Peak.rko lib/Minus.rko lib/Replay.rko lib/Kmeans.rko lib/Abs.rko lib/Sum.rko lib/Ctrl.rko lib/Diff.rko lib/Bang.rko lib/Macro.rko lib/MidiIn.rko lib/GLLua.rko lib/GLPlot.rko lib/Group.rko lib/System.rko
	
test/runner.cpp: $(TEST)
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)
	
test/runner: test/runner.cpp $(LINKER_OBJECTS) objects
	$(CC) $(CFLAGS) $(LFLAGS) -Itest $(INCLUDE_HEADERS) -I. test/runner.cpp $(LINKER_OBJECTS) -o test/runner

src/core/command.cpp: src/core/command.rl
	${RAGEL} src/core/command.rl -o src/core/command.cpp

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

src/lib/osc/liboscpack.a:
	cd src/lib/osc && make liboscpack.a
	
dot: src/core/command.png	
	open src/core/command.png

src/core/command.dot: src/core/command.rl
	ragel src/core/command.rl | rlgen-dot -p -o src/core/command.dot
	
src/core/command.png: src/core/command.dot
	${DOT_CMD} -Tpng src/core/command.dot -o src/core/command.png

install: install_rubyk install_libs

install_rubyk:
	$(MKDIR) $(BINDIR)
	$(INSTALL) rubyk $(BINDIR)

install_libs:
	$(MKDIR) $(RUBYK_LIBDIR)
	cd lib && for e in *.rko ; do echo $(RUBYK_LIBDIR)/$$e && $(INSTALL) $$e $(RUBYK_LIBDIR) ; done

clean:
	rm -rf *.o *.dSYM lib/*.rko lib/*.dSYM rubyk src/core/command.png src/core/command.dot test/runner test/runner.cpp test/*.dSYM
