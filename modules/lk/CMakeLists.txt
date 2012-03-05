set(MODULE_NAME lk)
# --------------------------------------------------------------

include_directories(include)
include_directories(core)

# --------------------------------------------------------------
#  module
# --------------------------------------------------------------
add_custom_target(${MODULE_NAME} true)
file(MAKE_DIRECTORY ${LUBYK_LIB}/${MODULE_NAME})

# --------------------------------------------------------------
#  module_core                                 module/core.so
# --------------------------------------------------------------
file(GLOB CORE_SOURCES core/dub/*.cpp core/*.cpp core/${PLAT}/*.cpp core/${PLAT}/*.mm)
add_library("${MODULE_NAME}_core" MODULE ${CORE_SOURCES})
set_target_properties("${MODULE_NAME}_core"
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_LIB}/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
target_link_libraries("${MODULE_NAME}_core" ${PLAT_LINK} -lavahi-client -lpthread)
add_dependencies(${MODULE_NAME} "${MODULE_NAME}_core")

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

