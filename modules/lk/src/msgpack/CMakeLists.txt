include_directories(vendor/src)

file(GLOB MSGPACK_SOURCES vendor/src/*.c msgpack.cpp)
add_library(msgpack ${MSGPACK_SOURCES})
