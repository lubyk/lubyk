# ----------------------------------------------------------------------------
#  CMake file for Lua. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(lua_addons)

file (GLOB lib_includes ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(REMOVE_ITEM lib_includes ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
include_directories(${lib_includes} ${RUBYK_SOURCE_DIR}/include/rubyk/lua)

# ----------------------------------------------------------------------------------
# 				Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "lua_addons")

file(GLOB lib_srcs ${CMAKE_CURRENT_SOURCE_DIR}/*/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)
file(GLOB lib_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h)
set(lib_ext_hdrs)

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs} ${lib_ext_hdrs})

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	LINK_FLAGS ${PLAT_LINK}
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)
