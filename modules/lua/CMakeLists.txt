project(luabin C)
set(target "lua")

#add_definitions("-DLUA_USE_MACOSX -DLUA_USE_DLOPEN")
include_directories(vendor/lua5.1/src)

file(GLOB MODULE_SOURCES vendor/lua5.1/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua5.1/src/luac.c)

if(PLAT STREQUAL "macosx")
  set(EXECUTABLE_OUTPUT_PATH ${LUBYK_SOURCE_DIR}/deploy/macosx/Lubyk/Lubyk.app/Contents/Resources)
else(PLAT STREQUAL "macosx")
  message("OUTPUT PATH NOT DEFINED FOR ${PLAT} (lua target)")
  set(EXECUTABLE_OUTPUT_PATH ${LUBYK_SOURCE_DIR}/deploy/${PLAT})
endif(PLAT STREQUAL "macosx")

add_executable(${target} ${MODULE_SOURCES})
# We should only do this if we have a static readline.
target_link_libraries(${target} readline)

set_target_properties(${target} PROPERTIES OUTPUT_NAME "lubyk")
