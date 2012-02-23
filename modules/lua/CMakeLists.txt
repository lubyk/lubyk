project(luabin C)
set(target "lua")

add_definitions("-DLUA_USE_LINUX")

include_directories(vendor/lua5.1/src)

file(GLOB MODULE_SOURCES vendor/lua5.1/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua5.1/src/luac.c)

set(EXECUTABLE_OUTPUT_PATH ${LUBYK_LIB}/lubyk)

add_executable(${target} ${MODULE_SOURCES})
# We should only do this if we have a static readline.

if(LINUX)
  message("LINUX LUA")
  target_link_libraries(${target} readline dl m)
  # $(MAKE) all MYCFLAGS=-DLUA_USE_LINUX MYLIBS="-Wl,-E -ldl -lreadline -lhistory -lncurses"
else(LINUX)
  target_link_libraries(${target} readline)
endif(LINUX)

# name the executable lubyk
set_target_properties(${target} PROPERTIES OUTPUT_NAME "lubyk")
