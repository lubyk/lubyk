# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

#include_directories(.)

set(target "${MODULE_NAME}_slua")
add_custom_target(${target})

#copy_files(${target} LuaXml.lua ${LUBYK_LIB})
add_custom_command(
  TARGET  ${target}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LuaXml.lua ${LUBYK_LIB}
  )

add_dependencies(${MODULE_NAME} ${target})

set(SUB_MODULE_SOURCES LuaXML_lib.c)
build_sub_module("vendor" ${SUB_MODULE_SOURCES})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
