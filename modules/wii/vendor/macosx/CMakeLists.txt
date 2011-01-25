# ==============================================================================
#
#  Static build of the WiiRemoteFramework by Hiroaki Kimura
#
# ==============================================================================

list(APPEND SUB_MODULE_SOURCES Remote.mm WiiRemoteFramework/WiiRemote.m WiiRemoteFramework/WiiRemoteDiscovery.m)
message(">>>>>>>>${SUB_MODULE_SOURCES}")
include_directories(WiiRemoteFramework)

add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS} -framework Cocoa
  LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
#import <Cocoa/Cocoa.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
target_link_libraries("${target}_${SUB_MODULE_NAME}" "-framework Cocoa -framework IOBluetooth -framework CoreData -framework ApplicationServices -framework Foundation")
add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")
