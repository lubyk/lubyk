# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

#include_directories(.)

set(target "${MODULE_NAME}_slua")

add_custom_target(${target})
copy_files(${target} LuaXml.lua lib)

add_dependencies(${MODULE_NAME} ${target})

set(SUB_MODULE_SOURCES LuaXML_lib.c)
build_sub_module("vendor" ${SUB_MODULE_SOURCES})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
