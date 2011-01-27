# dummy makefile for those too lazy to read the README file...
# and for me when I do not want to go into the build directory.

default: build/MakeFile
	cd build && make

lubyk: build/MakeFile
	cd build && make lubyk

build/MakeFile: build
	cd build && cmake ..

build:
	mkdir build

clean: build
	cd build && make clean
