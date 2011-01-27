
# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

include_directories(src)

file(GLOB MSGPACK_SOURCES src/*.c)
set(SUB_MODULE_NAME "vendor")
set(SUB_MODULE_SOURCES ${MSGPACK_SOURCES} lubyk.c)


add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
add_dependencies (modules "${target}_${SUB_MODULE_NAME}")

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------