# ----------------------------------------------------------------------------
#  CMake file for Lua. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(luajit)

message("THIS DOES NOT WORK FOR THE MOMENT")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ----------------------------------------------------------------------------------
# 				Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "luajit")

file(GLOB lib_srcs src/*.c)
list (REMOVE_ITEM lib_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/ljamalg.c ${CMAKE_CURRENT_SOURCE_DIR}/src/luajit.c)
file(GLOB lib_hdrs src/*.h)
set(lib_ext_hdrs)

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs} ${lib_ext_hdrs})

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)

#add_definitions(-arch i386)
#set(CMAKE_OSX_ARCHITECTURES "i386" CACHE STRING "Build architectures for OSX" FORCE)
#message("Forcing i386 architecture to build with Lua JIT")
#message("   (you need to build oscit with cd vendor/oscit && mkdir build && cmake -D RELEASE=ON .. && make")
#set(LUA_DIR   "${RUBYK_SOURCE_DIR}/vendor/lua_jit2/src")
#set(LUA_LIB   "${LUA_DIR}/libluajit.a")
#set(LUA_BUILD cd ${RUBYK_SOURCE_DIR}/vendor/lua-jit2 && make)
#
#add_custom_command ( # PRE_BUILD
#  OUTPUT ${LUA_LIB}
#  COMMAND ${LUA_BUILD}
#)
#set (CUSTOM_BUILD_DEPENDS ${LUA_LIB})
#