# --------------------------------------------------------------
#  core
# --------------------------------------------------------------
file(GLOB CORE_SOURCES dub/*.cpp *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)
add_library("${MODULE_NAME}_core" MODULE ${CORE_SOURCES})
set_target_properties("${MODULE_NAME}_core"
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_LIB}/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)  
# msgpack needed for lk.Socket
# time_ref needed by lk.TimeRef
target_link_libraries("${MODULE_NAME}_core" msgpack time_ref lua_dump)

add_dependencies(${MODULE_NAME} "${MODULE_NAME}_core")
