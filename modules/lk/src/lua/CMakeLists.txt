project(luabin C)
set(target "luabin")

add_definitions("-DLUA_USE_LINUX")

include_directories(vendor/src)

file(GLOB MODULE_SOURCES vendor/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/src/luac.c)

# FIXME: This should be an install rule (different on macosx and linux).
set(EXECUTABLE_OUTPUT_PATH ${LUBYK_LIB}/lubyk)

add_executable(${target} ${MODULE_SOURCES})
# We should only do this if we have a static readline.

if(LINUX)
  message("LINUX LUA")
  target_link_libraries(${target} readline dl m pthread)
  # $(MAKE) all MYCFLAGS=-DLUA_USE_LINUX MYLIBS="-Wl,-E -ldl -lreadline -lhistory -lncurses"
else(LINUX)
  target_link_libraries(${target} readline)
endif(LINUX)

# name the executable lubyk
set_target_properties(${target} PROPERTIES OUTPUT_NAME "lubyk")

