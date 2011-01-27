# ==============================================================================
#
#  Static build of the WiiRemoteFramework by Hiroaki Kimura
#
# ==============================================================================
set(SUB_MODULE_NAME "core")
file(GLOB SUB_MODULE_SOURCES ${SUB_MODULE}/*.cpp ${SUB_MODULE}/${PLAT}/*.cpp ${SUB_MODULE}/${PLAT}/*.mm)
add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS} -framework Cocoa
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)

#import <Cocoa/Cocoa.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
target_link_libraries("${target}_${SUB_MODULE_NAME}" "-framework Cocoa")
add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")
