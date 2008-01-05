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


# Utilities.
INSTALL= install -C
MKDIR= mkdir -p

test: test/runner test/runner.cpp
	./test/runner && rm test/runner

rubyk: classes/main.cpp command.o rubyk.o signal.o node.o inlet.o outlet.o slot.o params.o class.o lua.o matrix.o buffer.o classes/lua/src/liblua.a objects
	$(CC) $(CFLAGS) $(LFLAGS) -o rubyk -Itest -Itemplates -Iclasses -Iobjects -I. classes/main.cpp slot.o inlet.o outlet.o params.o signal.o node.o class.o command.o matrix.o buffer.o rubyk.o lua.o classes/lua/src/liblua.a

objects: lib/Test.rko lib/Add.rko lib/Value.rko lib/Counter.rko lib/Metro.rko lib/Print.rko lib/Midi.rko lib/NoteOut.rko lib/Lua.rko lib/Serial.rko lib/Turing.rko lib/Keyboard.rko lib/Cabox.rko lib/Svm.rko lib/Buffer.rko lib/Plot.rko lib/Cut.rko lib/MaxCount.rko lib/FFT.rko lib/VQ.rko lib/ClassRecorder.rko lib/PCA.rko lib/Average.rko lib/Peak.rko lib/Minus.rko lib/Replay.rko lib/Kmeans.rko lib/Abs.rko lib/Sum.rko lib/Ctrl.rko lib/Diff.rko lib/Bang.rko
	
test/runner.cpp: test/*_test.h test/objects/*_test.h
	./test/cxxtest/cxxtestgen.pl --error-printer -o test/runner.cpp $(TEST)
	
test/runner: test/runner.cpp command.o rubyk.o signal.o node.o inlet.o outlet.o slot.o params.o class.o lua.o classes/lua/src/liblua.a matrix.o buffer.o objects
	$(CC) $(CFLAGS) $(LFLAGS) -Itest -Itemplates -Iclasses -Iobjects -I. test/runner.cpp slot.o inlet.o outlet.o params.o signal.o node.o class.o command.o rubyk.o lua.o classes/lua/src/liblua.a matrix.o buffer.o -o test/runner
	
signal.o: classes/rubyk_signal.cpp
	$(CC) $(CFLAGS) -c -Itemplates classes/rubyk_signal.cpp -o signal.o

classes/command.cpp: classes/command.rl
	${RAGEL} classes/command.rl | ${RAGEL_CD} -o classes/command.cpp

rubyk.o: classes/rubyk.h classes/rubyk.cpp classes/mutex.h
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates classes/rubyk.cpp -o rubyk.o

command.o: classes/command.cpp classes/command.h classes/mutex.h
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates classes/command.cpp -o command.o

%.o: classes/%.cpp
	$(CC) $(CFLAGS) -c -Iclasses -Itemplates $< -o $@
	
lib/Midi.rko: objects/Midi.cpp
	$(CC) $(CFLAGS) -o lib/Midi.rko -Itemplates -Iclasses -Iobjects -dynamic -bundle -undefined suppress -flat_namespace -D__MACOSX_CORE__ -L/usr/lib -lgcc -lstdc++ -framework CoreMIDI -framework CoreFoundation -framework CoreAudio  objects/rtmidi/RtMidi.cpp objects/Midi.cpp

lib/Serial.rko: objects/Serial.cpp objects/serial/serial.h
	$(CC) $(CFLAGS) -o lib/Serial.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Serial.cpp -DCOMPILE_SERIAL_OBJECT

objects/Turing.cpp: objects/Turing.rl
	${RAGEL} objects/Turing.rl | ${RAGEL_CD} -o objects/Turing.cpp
	
lib/Svm.rko: objects/Svm.cpp
	$(CC) $(CFLAGS) -o lib/Svm.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/Svm.cpp objects/svm/svm.cpp
	
lib/Plot.rko: objects/Plot.cpp classes/opengl.h
	$(CC) $(CFLAGS) -o lib/Plot.rko -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ -D__MACOSX_CORE__ -framework GLUT -framework OpenGL -framework Cocoa objects/Plot.cpp

lib/FFT.rko: objects/FFT.cpp
	$(CC) $(CFLAGS) -o lib/FFT.rko -Itemplates -Iclasses -Iobjects/fft -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/FFT.cpp
	
lib/VQ.rko: objects/VQ.cpp
	$(CC) $(CFLAGS) -o lib/VQ.rko -Itemplates -Iclasses -Iobjects/vq -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ objects/VQ.cpp objects/vq/elbg.c objects/vq/random.c

lib/%.rko: objects/%.cpp
	$(CC) $(CFLAGS) -o $@ -Itemplates -Iclasses -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ $<


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

install: install_rubyk install_libs

install_rubyk:
	$(MKDIR) $(BINDIR)
	$(INSTALL) rubyk $(BINDIR)

install_libs:
	$(MKDIR) $(RUBYK_LIBDIR)
	cd lib && for e in *.rko ; do echo $(RUBYK_LIBDIR)/$$e && $(INSTALL) $$e $(RUBYK_LIBDIR) ; done

clean:
	rm -rf *.o lib/*.rko rubyk classes/command.png classes/command.dot test/runner