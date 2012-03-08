
include_directories(vendor/include)
include_directories(core)

# creates zmq_vendor
add_subdirectory(vendor)

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
# msgpack and time_ref are in lk/src
# zmq_vendor is created by vendor
target_link_libraries("${MODULE_NAME}_core" ${PLAT_LINK} msgpack time_ref zmq_vendor)
add_dependencies(${MODULE_NAME} "${MODULE_NAME}_core")

