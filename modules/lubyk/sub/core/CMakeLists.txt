# ==============================================================================
#
#  Static build of the WiiRemoteFramework by Hiroaki Kimura
#
# ==============================================================================
#set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework Cocoa")
build_sub_module_with_defaults()
#target_link_libraries("${MODULE_NAME}_core" "-framework Cocoa")
target_link_libraries("${MODULE_NAME}_core")
