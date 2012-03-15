include_directories(core)

# --------------------------------------------------------------
#  lua
# --------------------------------------------------------------
add_subdirectory(lua)

# --------------------------------------------------------------
#  lua_dump
# --------------------------------------------------------------
add_subdirectory(lua_dump)

# --------------------------------------------------------------
#  msgpack
# --------------------------------------------------------------
add_subdirectory(msgpack)

# --------------------------------------------------------------
#  time_ref
# --------------------------------------------------------------
add_subdirectory(time_ref)

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
target_link_libraries("${MODULE_NAME}_core" ${PLAT_LINK} msgpack)

add_dependencies(${MODULE_NAME} "${MODULE_NAME}_core")
