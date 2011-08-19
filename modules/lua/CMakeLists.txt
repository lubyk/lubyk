project(luabin C)
set(target "lua")

add_definitions(-DLUA_USE_LINUX)
include_directories(vendor/lua5.1/src)

file(GLOB MODULE_SOURCES vendor/lua5.1/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua5.1/src/luac.c)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_definitions(-O2 -Wall)

add_executable(${target} ${MODULE_SOURCES})
target_link_libraries(${target} readline)
