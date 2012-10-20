# TODO: we could use https://github.com/LuaDist/luajit instead.
project(luabin C)
set(target "luabin")

# We do not use LUA_USE_LINUX implies readline dependency.
add_definitions("-DLUA_USE_LINUX")
#add_definitions("-DLUA_USE_POSIX -DLUA_USE_DLOPEN")

include_directories(vendor/src)

file(GLOB MODULE_SOURCES vendor/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/src/luac.c)

add_executable(${target} ${MODULE_SOURCES})
add_executable(${target}_lubyk ${MODULE_SOURCES})

target_link_libraries(${target} readline)
target_link_libraries(${target}_lubyk readline)

# We could link to readline if we had a static non-gnu version.
if(LINUX)
  target_link_libraries(${target} dl m pthread)
  target_link_libraries(${target}_lubyk dl m pthread)
else(LINUX)
  #target_link_libraries(${target} )
endif(LINUX)

# name the executable lubyk
set_target_properties(${target}_lubyk PROPERTIES OUTPUT_NAME "lubyk")
set_target_properties(${target}       PROPERTIES OUTPUT_NAME "lua")

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
INSTALL(TARGETS ${target} ${target}_lubyk
  RUNTIME DESTINATION bin
)

if(APPLE)
  # Copy 'lubyk' executable inside Lubyk.app
  install(TARGETS ${target}_lubyk
    DESTINATION /Applications/Lubyk.app/Contents/Resources
  )
endif(APPLE)
