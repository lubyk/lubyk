include_directories(${CMAKE_CURRENT_SOURCE_DIR} vendor/src)

file(GLOB MSGPACK_SOURCES vendor/src/*.c msgpack.cpp)
add_library(msgpack ${MSGPACK_SOURCES})
