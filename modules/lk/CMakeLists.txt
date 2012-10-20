set(MODULE_NAME lk)
# --------------------------------------------------------------

include_directories(include)

# --------------------------------------------------------------
#  module
# --------------------------------------------------------------
add_custom_target(${MODULE_NAME} true)

# --------------------------------------------------------------
#  src (module/core.so, msgpack.a, lua, lua_dump.a, time_ref.a
# --------------------------------------------------------------
add_subdirectory(src)

# --------------------------------------------------------------
#  module_bind                                   
# --------------------------------------------------------------
file(GLOB MODULE_HEADERS include/${MODULE_NAME}/*.h)
add_custom_target("${MODULE_NAME}_bind"
  COMMAND lua bind.lua
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${MODULE_HEADERS} bind.lua)

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
install(DIRECTORY lib
  DESTINATION ${LUBYK_LIB}
)

# Bootstrap
install(FILES install/${PLAT}/lubyk.lua
  DESTINATION share/lua/5.1)

# Needed when loading from local lib
install(FILES install/${PLAT}/lubyk.lua
  DESTINATION ${LUBYK_LIB}/lib)

install(FILES install/${PLAT}/install.sh
  DESTINATION ${LUBYK_LIB}/lib
  PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
