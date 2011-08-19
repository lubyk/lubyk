
# Found in a post by Ken Martin
set(EXECUTABLE_OUTPUT_PATH ${LUBYK_SOURCE_DIR})

add_definitions(-DLUA_USE_LINUX)

# define the lua core source files
set (LUA_CORE_SRCS lapi.c lcode.c ldebug.c ldo.c ldump.c
     lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
     lparser.c lstate.c lstring.c ltable.c ltm.c  lundump.c
     lvm.c lzio.c)

# define the lua lib source files
set (LUA_LIB_SRCS lauxlib.c lbaselib.c ldblib.c liolib.c
     lmathlib.c loslib.c ltablib.c lstrlib.c loadlib.c linit.c)

# create the library
add_library(lua EXCLUDE_FROM_ALL ${LUA_LIB_SRCS} ${LUA_CORE_SRCS})

# create the lua executable and link it to the lib
add_executable(luaexec EXCLUDE_FROM_ALL lua.c)
target_link_libraries(luaexec lua)

# name the executable lua just like the library
set_target_properties(luaexec PROPERTIES OUTPUT_NAME lua)

# create the luac thing (I have no clue) and link it
add_executable(luac EXCLUDE_FROM_ALL luac.c print.c)
target_link_libraries (luac lua readline)
