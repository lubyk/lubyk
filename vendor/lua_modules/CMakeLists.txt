# ----------------------------------------------------------------------------
#  CMake file for Lua. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(lua_modules)

file (GLOB LUA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(REMOVE_ITEM LUA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

include_directories(${lib_includes}
  ${RUBYK_SOURCE_DIR}/include
  ${RUBYK_SOURCE_DIR}/include/rubyk/lua
  ${RUBYK_SOURCE_DIR}/include/rubyk/support
  ${RUBYK_SOURCE_DIR}/vendor/oscit/include)

# ----------------------------------------------------------------------------------
# 				Define the library target:
# ----------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "-fno-common")
#
# g++ -O2 -fno-common -c -o test.o -I../../include/rubyk/lua lua_module.cpp
# gcc -bundle -undefined dynamic_lookup -o test.so test.o -lstdc++

# replace luaopen_xx by luaload_xx
add_definitions(-DDUB_LUA_NO_OPEN)

foreach (LUA_MODULE ${LUA_MODULES})
  # get name
  get_filename_component (LUA_MODULE_NAME ${LUA_MODULE} NAME_WE)
  set(the_target "lua_${LUA_MODULE_NAME}")
  # get sources
  file (GLOB LUA_MODULE_SOURCES ${LUA_MODULE}/*.cpp)
  # create name.so library
  add_library(${the_target} MODULE ${LUA_MODULE_SOURCES})
  set_target_properties(${the_target}
    PROPERTIES OUTPUT_NAME ${LUA_MODULE_NAME}
    LINK_FLAGS ${PLAT_OBJ_FLAGS}
    LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/build/lib/lua
    PREFIX ""
    SUFFIX ".so"
  )
  add_dependencies (lua_modules ${the_target})
endforeach (LUA_MODULE)

