# TODO: we could use https://github.com/LuaDist/luajit instead.
project(luabin C)
set(target "luabin")

if(USE_LUAJIT)
  #--=============================================== Luajit
  set(JIT_OUTPUT ${CMAKE_BINARY_DIR}/lubyk)

  add_custom_command(
    OUTPUT ${JIT_OUTPUT}
    COMMAND echo "Building luajit can take some time, please wait..."
    COMMAND make clean && make -j4
    COMMAND cp src/luajit ${CMAKE_BINARY_DIR}/lubyk
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor_jit
    )
  add_custom_target("luabin_clean"
    COMMAND make clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor_jit
    )
  add_custom_target(${target} ALL DEPENDS ${JIT_OUTPUT})

  # --------------------------------------------------------------
  #  install                                     
  # --------------------------------------------------------------

  if(APPLE)
    # Copy 'lubyk' executable inside Lubyk.app
    install(FILES ${CMAKE_BINARY_DIR}/lubyk
      DESTINATION /Applications/Lubyk.app/Contents/Resources
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
  else(APPLE)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/lubyk
      RUNTIME DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
  endif(APPLE)

else(USE_LUAJIT)
  #--=============================================== Puc-Rio Lua
  # We do not use LUA_USE_LINUX implies readline dependency.
  add_definitions("-DLUA_USE_LINUX")
  #add_definitions("-DLUA_USE_POSIX -DLUA_USE_DLOPEN")

  include_directories(vendor/src)

  file(GLOB MODULE_SOURCES vendor/src/*.c)
  list(REMOVE_ITEM MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/src/luac.c)

  add_executable(${target} ${MODULE_SOURCES})

  target_link_libraries(${target} readline)

  # We could link to readline if we had a static non-gnu version.
  if(LINUX)
    target_link_libraries(${target} dl m pthread)
  else(LINUX)
    #target_link_libraries(${target} )
  endif(LINUX)

  # name the executable lubyk
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "lubyk")

  # --------------------------------------------------------------
  #  install                                     
  # --------------------------------------------------------------

  if(APPLE)
    # Copy 'lubyk' executable inside Lubyk.app
    install(TARGETS ${target}
      DESTINATION /Applications/Lubyk.app/Contents/Resources
      )
  else(APPLE)
    INSTALL(TARGETS ${target}
      RUNTIME DESTINATION bin
      )
  endif(APPLE)

endif(USE_LUAJIT)
