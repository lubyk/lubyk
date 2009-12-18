# dummy makefile for those too lazy to read the README file...

default: build build/MakeFile
	cd build && make

build:
	mkdir build

build/MakeFile: build
	cd build && cmake ..

clean: build
	cd build && make clean

oscit: build build/MakeFile
	cd build && make oscit

examples: build build/MakeFile
	cd build && make examples
