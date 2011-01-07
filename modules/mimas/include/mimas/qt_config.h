
// special static build
// NOT binary compatible with installed Qt frameworks
// build Qt with
// > ./configure -static -release
// > make
// > make install --> install static lib in /usr/local/Trolltech/Qt-4.7.2
// If qt_config.h is changed, rebuild headers and reconfigure with
// [I think this is not needed]> rm -rf path/to/qt/include
// > make confclean
// > ...