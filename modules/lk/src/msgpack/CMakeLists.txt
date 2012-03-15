include_directories(vendor/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB MSGPACK_SOURCES vendor/src/*.c msgpack.cpp)
add_library(msgpack ${MSGPACK_SOURCES})
