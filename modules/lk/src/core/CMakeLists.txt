# --------------------------------------------------------------
#  core
# --------------------------------------------------------------
file(GLOB CORE_SOURCES core/dub/*.cpp core/*.cpp core/${PLAT}/*.cpp core/${PLAT}/*.mm)
add_library("${MODULE_NAME}_core" MODULE ${CORE_SOURCES})
set_target_properties("${MODULE_NAME}_core"
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_LIB}/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)  
# msgpack needed for Socket
# target_link_libraries("${MODULE_NAME}_core" msgpack)
