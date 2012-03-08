set(MODULE_NAME zmq)
# --------------------------------------------------------------
#  module
# --------------------------------------------------------------
add_custom_target(${MODULE_NAME} true)
file(MAKE_DIRECTORY ${LUBYK_LIB}/${MODULE_NAME})

# --------------------------------------------------------------
#  src
# --------------------------------------------------------------
include_directories(include)
add_subdirectory(src)

# --------------------------------------------------------------
#  module_bind                                   
# --------------------------------------------------------------
file(GLOB MODULE_HEADERS include/${MODULE_NAME}/*.h)
add_custom_target("${MODULE_NAME}_bind"
  COMMAND lua bind.lua
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${MODULE_HEADERS} bind.lua)

# --------------------------------------------------------------
#  module_lib                                  module/xxxx.lua
# --------------------------------------------------------------
add_custom_target("${MODULE_NAME}_lib" ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib ${LUBYK_LIB}
)
add_dependencies(${MODULE_NAME} "${MODULE_NAME}_lib")


