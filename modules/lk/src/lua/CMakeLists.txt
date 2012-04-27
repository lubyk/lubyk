project(luabin C)
set(target "luabin")

add_definitions("-DLUA_USE_LINUX")

include_directories(vendor/src)

file(GLOB MODULE_SOURCES vendor/src/*.c)
list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/src/luac.c)

add_executable(${target} ${MODULE_SOURCES})
# We should only do this if we have a static readline.

if(LINUX)
  target_link_libraries(${target} readline dl m pthread)
else(LINUX)
  target_link_libraries(${target} readline)
endif(LINUX)

# name the executable lubyk
set_target_properties(${target} PROPERTIES OUTPUT_NAME "lubyk")

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
INSTALL(TARGETS ${target}
  RUNTIME DESTINATION bin
)

if(APPLE)
  # Copy 'lubyk' executable inside Lubyk.app
  install(TARGETS ${target}
    DESTINATION /Applications/Lubyk.app/Contents/Resources
  )
endif(APPLE)
