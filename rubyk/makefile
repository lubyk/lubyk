CC=g++
CFLAGS=-g -Wall -DUSE_READLINE
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
LINKER_OBJECTS=slot.o inlet.o outlet.o params.o signal.o node.o class.o command.o matrix.o buffer.o rubyk.o lua_script.o src/lib/lua/liblua.a globals.o

# Utilities.
INSTALL= install -C
MKDIR= mkdir -p

test: test/runner test/runner.cpp rubyk
	./test/runner && rm test/runner

rubyk: src/core/main.cpp $(LINKER_OBJECTS) objects
	$(CC) $(CFLAGS) $(LFLAGS) -o rubyk -Itest -Isrc/templates -Isrc/core -Isrc/objects -I. src/core/main.cpp $(LINKER_OBJECTS)

objects: lib/Test.rko lib/Add.rko lib/Value.rko lib/Counter.rko lib/Metro.rko lib/Print.rko lib/MidiOut.rko lib/NoteOut.rko lib/Lua.rko lib/Serial.rko lib/Turing.rko lib/Keyboard.rko lib/Cabox.rko lib/Svm.rko lib/Buffer.rko lib/GLWindow.rko lib/Cut.rko lib/MaxCount.rko lib/FFT.rko lib/VQ.rko lib/ClassRecorder.rko lib/PCA.rko lib/Average.rko lib/Peak.rko lib/Minus.rko lib/Replay.rko lib/Kmeans.rko lib/Abs.rko lib/Sum.rko lib/Ctrl.rko lib/Diff.rko lib/Bang.rko lib/Macro.rko lib/MidiIn.rko lib/GLLua.rko lib/GLPlot.rko
	
test/runner.cpp: $(TEST)
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)
	
test/runner: test/runner.cpp $(LINKER_OBJECTS) objects
	$(CC) $(CFLAGS) $(LFLAGS) -Itest -Isrc/templates -Isrc/core -Isrc/objects -I. test/runner.cpp $(LINKER_OBJECTS) -o test/runner
	
signal.o: src/core/rubyk_signal.cpp
	$(CC) $(CFLAGS) -c -Isrc/templates src/core/rubyk_signal.cpp -o signal.o

src/core/command.cpp: src/core/command.rl
	${RAGEL} src/core/command.rl -o src/core/command.cpp

rubyk.o: src/core/rubyk.h src/core/rubyk.cpp src/core/mutex.h
	$(CC) $(CFLAGS) -c -Isrc/core -Isrc/templates src/core/rubyk.cpp -o rubyk.o

command.o: src/core/command.cpp src/core/command.h src/core/mutex.h
	$(CC) $(CFLAGS) -c -Isrc/core -Isrc/templates src/core/command.cpp -o command.o

%.o: src/core/%.cpp
	$(CC) $(CFLAGS) -c -Isrc/core -Isrc/templates $< -o $@
	
lib/MidiOut.rko: src/objects/MidiOut.cpp
	$(CC) $(CFLAGS) -o lib/MidiOut.rko -Isrc/templates -Isrc/core -Isrc/objects -dynamic -bundle -undefined suppress -flat_namespace -D__MACOSX_CORE__ -L/usr/lib -lgcc -lstdc++ -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  src/objects/midi/RtMidi.cpp src/objects/MidiOut.cpp
	
lib/MidiIn.rko: src/objects/MidiIn.cpp
	$(CC) $(CFLAGS) -o lib/MidiIn.rko -Isrc/templates -Isrc/core -Isrc/objects -dynamic -bundle -undefined suppress -flat_namespace -D__MACOSX_CORE__ -L/usr/lib -lgcc -lstdc++ -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  src/objects/midi/RtMidi.cpp src/objects/MidiIn.cpp

lib/Serial.rko: src/objects/Serial.cpp src/objects/serial/serial.h
	$(CC) $(CFLAGS) -o lib/Serial.rko -Isrc/templates -Isrc/core -Isrc/lib/lua -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ src/objects/Serial.cpp -DCOMPILE_SERIAL_OBJECT

src/objects/Turing.cpp: src/objects/Turing.rl
	${RAGEL} src/objects/Turing.rl -o src/objects/Turing.cpp
	
lib/Svm.rko: src/objects/Svm.cpp
	$(CC) $(CFLAGS) -o lib/Svm.rko -Isrc/templates -Isrc/core -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ src/objects/Svm.cpp src/objects/svm/svm.cpp

lib/FFT.rko: src/objects/FFT.cpp
	$(CC) $(CFLAGS) -o lib/FFT.rko -Isrc/templates -Isrc/core -Isrc/objects/fft -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ src/objects/FFT.cpp
	
lib/VQ.rko: src/objects/VQ.cpp
	$(CC) $(CFLAGS) -o lib/VQ.rko -Isrc/templates -Isrc/core -Isrc/objects/vq -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ src/objects/VQ.cpp src/objects/vq/elbg.c src/objects/vq/random.c
	
lib/GLLua.rko: src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c
	$(CC) $(CFLAGS) -o lib/GLLua.rko -Isrc/templates -Isrc/core -Isrc/lib/lua -Isrc/objects/gllua -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ -D__MACOSX_CORE__ -framework GLUT -framework OpenGL -framework Cocoa src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c
  
lib/GL%.rko: src/objects/GL%.cpp
	$(CC) $(CFLAGS) -o $@ -Isrc/templates -Isrc/core -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ -D__MACOSX_CORE__ -framework GLUT -framework OpenGL -framework Cocoa $<
	
lib/%.rko: src/objects/%.cpp
	$(CC) $(CFLAGS) -o $@ -Isrc/templates -Isrc/core -Isrc/lib/lua -D__MACOSX_CORE__ -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ $<

lua_script.o: src/core/lua_script.cpp src/core/lua_script.h src/core/script.h
	$(CC) $(CFLAGS) -c -o lua_script.o -Isrc/templates -Isrc/core -Isrc/lib/lua src/core/lua_script.cpp

src/lib/lua/liblua.a:
	cd src/lib/lua && make $(PLAT)
	
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
	rm -rf *.o lib/*.rko rubyk src/core/command.png src/core/command.dot test/runner
