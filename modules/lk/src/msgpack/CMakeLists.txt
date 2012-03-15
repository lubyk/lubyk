include_directories(vendor/src)

file(GLOB MSGPACK_SOURCES vendor/src/*.c msgpack.cpp)
add_library(msgpack ${MSGPACK_SOURCES})
set_target_properties(msgpack
  PROPERTIES
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
)
