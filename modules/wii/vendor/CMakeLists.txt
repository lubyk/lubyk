# ==============================================================================
#
#  Wii library
#
# ==============================================================================
if(UNIX)
  if(APPLE)
    #  Use WiiRemoteFramework by Hiroaki Kimura
    set(SUB_MODULE_SOURCES lubyk.cpp ${PLAT}/Remote.mm ${PLAT}/WiiRemoteFramework/WiiRemote.m ${PLAT}/WiiRemoteFramework/WiiRemoteDiscovery.m)
    #set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework Cocoa")
    include_directories(${PLAT}/WiiRemoteFramework)

    build_sub_module("vendor" ${SUB_MODULE_SOURCES})
    target_link_libraries("${MODULE_NAME}_vendor" "-framework Cocoa -framework IOBluetooth -framework CoreData -framework ApplicationServices -framework Foundation")
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)
