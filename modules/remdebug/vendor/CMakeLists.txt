# [[ LUBYK

# Simply install lua files
# ----------------------------------------------------------------------------------
#  module_vendor
# ----------------------------------------------------------------------------------

add_custom_target("${MODULE_NAME}_vendorlib" 
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/remdebug ${LUBYK_LIB}/remdebug)
add_custom_target("${MODULE_NAME}_vendor" 
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/controller.lua ${LUBYK_LIB}/remdebug/
  DEPENDS ${MODULE_NAME}_vendorlib
)
add_dependencies(${MODULE_NAME} "${MODULE_NAME}_vendor")

# ]]
