# --------------------------------------------------------------
#  zmq_vendor
#  
#  expects:
#    MODULE_NAME defined in the CMakeLists for the module
#
#  provides:
#    zmq/core.so
# --------------------------------------------------------------
set(target "${MODULE_NAME}_core")

include_directories(vendor/include)
include_directories(core)

# --------------------------------------------------------------
#  zmq_vendor (static)
# --------------------------------------------------------------
add_subdirectory(vendor)

# --------------------------------------------------------------
#  module_core                                 module/core.so
# --------------------------------------------------------------
file(GLOB CORE_SOURCES core/dub/*.cpp core/*.cpp core/${PLAT}/*.cpp core/${PLAT}/*.mm)
add_library(${target} MODULE ${CORE_SOURCES})
set_target_properties(${target}
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  PREFIX ""
  SUFFIX ".so"
)  
# msgpack and time_ref are in lk/src
# zmq_vendor is created by vendor
target_link_libraries(${target} ${PLAT_LINK} msgpack time_ref zmq_vendor)
add_dependencies(${MODULE_NAME} ${target})

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
install(TARGETS ${target}
  DESTINATION ${LUBYK_LIB}/lib/${MODULE_NAME}
)

